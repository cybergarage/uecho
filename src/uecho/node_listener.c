/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node_internal.h>
#include <uecho/core/server.h>
#include <uecho/core/observer.h>

/****************************************
 * uecho_object_notifyrequestproperty
 ****************************************/

void uecho_object_notifyrequestproperty(uEchoObject *obj, uEchoEsv esv, uEchoProperty *msgProp)
{
  uEchoObjectPropertyObserver *obs;
  
  if (!obj || !msgProp)
    return;
  
  for (obs = uecho_object_property_observer_manager_getobservers(obj->propListenerMgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (uecho_property_getcode(msgProp) != uecho_object_property_observer_getpropetycode(obs))
      continue;
    obs->listener(obj, esv, msgProp);
  }
}

/****************************************
 * uecho_message_isrequestesv
 ****************************************/

bool uecho_message_isrequestesv(uEchoEsv esv)
{
  if ((uEchoEsvWriteRequest <= esv) && (esv <= uEchoEsvNotificationRequest))
    return true;
  
  if (esv == uEchoEsvWriteReadRequest)
    return true;
  
  if (esv == uEchoEsvNotificationResponseRequired)
    return true;
  
  return false;
}

/****************************************
 * uecho_object_responsemessage
 ****************************************/

bool uecho_object_responsemessage(uEchoObject *obj, uEchoMessage *msg)
{
  uEchoNode *parentNode;
  uEchoProperty *msgProp, *nodeProp, *resProp;
  uEchoPropertyCode msgPropCode;
  int nodePropSize;
  byte *nodePropData;
  uEchoEsv msgEsv, resEsv;
  int msgOpc, n;
  uEchoMessage *resMsg;
  byte *resMsgBytes;
  size_t resMsgLen;
  
  if (!obj || !msg)
    return false;
  
  msgEsv = uecho_message_getesv(msg);
  if (!uecho_message_requestesv2responseesv(msgEsv, &resEsv))
    return false;

  parentNode = uecho_object_getparentnode(obj);
  if (!parentNode)
    return false;
  
  // Create response message
  
  resMsg = uecho_message_new();
  if (!resMsg)
    return false;
  
  uecho_message_setesv(resMsg, resEsv);
  uecho_message_setsourceobjectcode(resMsg, uecho_message_getdestinationobjectcode(msg));
  uecho_message_setdestinationobjectcode(resMsg, uecho_message_getsourceobjectcode(msg));
  
  msgOpc = uecho_message_getopc(msg);
  for (n=0; n<msgOpc; n++) {
    msgProp = uecho_message_getproperty(msg, n);
    if (!msgProp)
      continue;
    
    msgPropCode = uecho_property_getcode(msgProp);
    nodeProp = uecho_object_getproperty(obj, msgPropCode);
    if (!nodeProp)
      continue;
    
    if (!uecho_property_isreadable(nodeProp))
      continue;
    
    resProp = uecho_property_new();
    if (!resProp)
      continue;
    
    nodePropSize = uecho_property_getdatasize(nodeProp);
    nodePropData = uecho_property_getdata(nodeProp);
    uecho_property_setcode(resProp, msgPropCode);
    uecho_property_setdata(resProp, nodePropData, nodePropSize);
    
    uecho_message_addproperty(resMsg, resProp);
  }
  
  // Send response message
  
  resMsgBytes = uecho_message_getbytes(resMsg);
  resMsgLen = uecho_message_size(resMsg);
  if (resEsv == uEchoEsvNotification) {
    uecho_node_announcemessagebytes(parentNode, resMsgBytes, resMsgLen);
  }
  else {
    uecho_node_sendmessagebytes(parentNode, uecho_message_getsourceaddress(msg), resMsgBytes, resMsgLen);
  }
  
  uecho_message_delete(resMsg);

  return true;
}

/****************************************
 * uecho_object_handlemessage
 ****************************************/

void uecho_object_handlemessage(uEchoObject *obj, uEchoMessage *msg)
{
  uEchoProperty *msgProp, *nodeProp;
  uEchoPropertyCode msgPropCode;
  int msgPropSize;
  byte *msgPropData;
  uEchoEsv msgEsv;
  int msgOpc, n;
  
  if (!obj || !msg)
    return;
  
  msgEsv = uecho_message_getesv(msg);
  msgOpc = uecho_message_getopc(msg);
  
  // Write properties
  
  if (uecho_message_iswriterequest(msg)) {
    for (n=0; n<msgOpc; n++) {
      msgProp = uecho_message_getproperty(msg, n);
      if (!msgProp)
        continue;
      
      msgPropCode = uecho_property_getcode(msgProp);
      
      nodeProp = uecho_object_getproperty(obj, msgPropCode);
      if (!nodeProp)
        continue;
      
      if (!uecho_property_iswritable(nodeProp))
        continue;
      
      msgPropSize = uecho_property_getdatasize(msgProp);
      msgPropData = uecho_property_getdata(msgProp);
      
      uecho_property_setdata(nodeProp, msgPropData, msgPropSize);
    }
  }
  
  // Notify observers
  
  if (obj->allMsgListener) {
    obj->allMsgListener(obj, msg);
  }
  
  for (n=0; n<msgOpc; n++) {
    msgProp = uecho_message_getproperty(msg, n);
    if (!msgProp)
      continue;
    uecho_object_notifyrequestproperty(obj, msgEsv, msgProp);
  }

  // Response required ?
 
  if (uecho_message_isresponserequired(msg)) {
    uecho_object_responsemessage(obj, msg);
  }
}

/****************************************
 * uecho_node_servermessagelistener
 ****************************************/

bool uecho_node_isselfobjectmessage(uEchoNode *node, uEchoMessage *msg)
{
  if (!uecho_node_isaddress(node, uecho_message_getsourceaddress(msg)))
    return false;
      
  if (uecho_message_getsourceobjectcode(msg) != uecho_message_getdestinationobjectcode(msg))
    return false;
  
  return true;
}

/****************************************
* uecho_node_servermessagelistener
****************************************/

void uecho_node_servermessagelistener(uEchoServer *server, uEchoMessage *msg)
{
  uEchoEsv esv;
  uEchoNode *node;
  uEchoObjectCode dstObjCode;
  uEchoObject *nodeDestObj;

  if (!server || !msg)
    return;
  
  node = (uEchoNode *)uecho_server_getuserdata(server);
  if (!node)
    return;
  
  if (node->msgListener) {
    node->msgListener(node, msg);
  }

  // Ignore when the sounrce object equals the destination object
  
  // Disable for testing
  // if (uecho_node_isselfobjectmessage(node, msg))
  //   return;
  
  // Processing when the controlled object exists, except when ESV = 0x60-0x63, 0x6E and 0x74

  esv = uecho_message_getesv(msg);
  if (!uecho_message_isrequestesv(esv))
    return;
  
  // Processing when the controlled object does not exist
  
  dstObjCode = uecho_message_getdestinationobjectcode(msg);
  nodeDestObj = uecho_node_getobjectbycode(node, dstObjCode);
  if (!nodeDestObj)
    return;

    uecho_object_handlemessage(nodeDestObj, msg);
}
