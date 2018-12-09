/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node_internal.h>

#include <uecho/core/observer.h>
#include <uecho/core/server.h>

/****************************************
 * uecho_object_notifyrequestproperty
 ****************************************/

void uecho_object_notifyrequestproperty(uEchoObject* obj, uEchoEsv esv, uEchoProperty* msgProp)
{
  uEchoObjectPropertyObserver* obs;

  if (!obj || !msgProp)
    return;

  for (obs = uecho_object_property_observer_manager_getobservers(obj->propListenerMgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (esv != uecho_object_property_observer_getesv(obs))
      continue;
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

bool uecho_object_responsemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoNode* parentNode;
  uEchoProperty *msgProp, *nodeProp, *resProp;
  uEchoPropertyCode msgPropCode;
  int nodePropSize;
  byte* nodePropData;
  uEchoEsv msgEsv, resEsv;
  int msgOpc, n;
  uEchoMessage* resMsg;
  byte* resMsgBytes;
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
  uecho_message_settid(resMsg, uecho_message_gettid(msg));
  uecho_message_setsourceobjectcode(resMsg, uecho_message_getdestinationobjectcode(msg));
  uecho_message_setdestinationobjectcode(resMsg, uecho_message_getsourceobjectcode(msg));

  msgOpc = uecho_message_getopc(msg);
  for (n = 0; n < msgOpc; n++) {
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
 * uecho_object_responseerrormessage
 ****************************************/

bool uecho_object_responseerrormessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoNode* parentNode;
  uEchoEsv msgEsv, errEsv;
  uEchoMessage* errMsg;
  byte* errMsgBytes;
  size_t errMsgLen;

  if (!obj || !msg)
    return false;

  msgEsv = uecho_message_getesv(msg);
  if (!uecho_message_requestesv2errorresponseesv(msgEsv, &errEsv))
    return false;

  parentNode = uecho_object_getparentnode(obj);
  if (!parentNode)
    return false;

  // Create response error message

  errMsg = uecho_message_copy(msg);
  if (!errMsg)
    return false;

  uecho_message_setesv(errMsg, errEsv);
  uecho_message_setsourceobjectcode(errMsg, uecho_message_getdestinationobjectcode(msg));
  uecho_message_setdestinationobjectcode(errMsg, uecho_message_getsourceobjectcode(msg));

  // Send response message

  errMsgBytes = uecho_message_getbytes(errMsg);
  errMsgLen = uecho_message_size(errMsg);

  uecho_node_sendmessagebytes(parentNode, uecho_message_getsourceaddress(msg), errMsgBytes, errMsgLen);

  uecho_message_delete(errMsg);

  return true;
}

/****************************************
 * uecho_object_handlemessage
 ****************************************/

bool uecho_object_iswritablepropertyrequest(uEchoObject* obj, uEchoProperty* reqProp)
{
  uEchoProperty* nodeProp;

  nodeProp = uecho_object_getproperty(obj, uecho_property_getcode(reqProp));
  if (!nodeProp)
    return false;

  if (!uecho_property_iswritable(nodeProp))
    return false;

  if (uecho_property_getdatasize(reqProp) != uecho_property_getdatasize(nodeProp))
    return false;

  return true;
}

bool uecho_object_setpropertyrequest(uEchoObject* obj, uEchoProperty* reqProp)
{
  uEchoProperty* nodeProp;

  nodeProp = uecho_object_getproperty(obj, uecho_property_getcode(reqProp));
  if (!nodeProp)
    return false;

  return uecho_property_setdata(nodeProp, uecho_property_getdata(reqProp), uecho_property_getdatasize(reqProp));
}

void uecho_object_handlemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoProperty* msgProp;
  uEchoEsv msgEsv;
  int msgOpc, n;

  if (!obj || !msg)
    return;

  msgEsv = uecho_message_getesv(msg);
  msgOpc = uecho_message_getopc(msg);

  // Write properties

  if (uecho_message_iswriterequest(msg)) {
    for (n = 0; n < msgOpc; n++) {
      msgProp = uecho_message_getproperty(msg, n);
      if (!msgProp)
        continue;

      if (!uecho_object_iswritablepropertyrequest(obj, msgProp))
        continue;

      uecho_object_setpropertyrequest(obj, msgProp);
    }
  }

  // Notify observers

  if (obj->allMsgListener) {
    obj->allMsgListener(obj, msg);
  }

  for (n = 0; n < msgOpc; n++) {
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

bool uecho_node_isselfobjectmessage(uEchoNode* node, uEchoMessage* msg)
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

bool uecho_property_isacceptablemessage(uEchoProperty* prop, uEchoMessage* msg)
{
  if (uecho_message_isreadrequest(msg) && uecho_property_isreadable(prop))
    return true;

  if (uecho_message_iswriterequest(msg) && uecho_property_iswritable(prop))
    return true;

  return false;
}

void uecho_node_servermessagelistener(uEchoServer* server, uEchoMessage* msg)
{
  uEchoEsv esv;
  uEchoNode* node;
  uEchoObjectCode msgDstObjCode;
  uEchoObject* msgDestObj;
  int msgOpc, n;
  uEchoProperty *msgProp, *nodeProp;

  if (!server || !msg)
    return;

  node = (uEchoNode*)uecho_server_getuserdata(server);
  if (!node)
    return;

  if (node->msgListener) {
    node->msgListener(node, msg);
  }

  // 4.2.2 Basic Sequences for Object Control in General
  // (A) | Processing when the controlled object does not exist

  msgDstObjCode = uecho_message_getdestinationobjectcode(msg);
  msgDestObj = uecho_node_getobjectbycode(node, msgDstObjCode);

  if (!msgDestObj)
    return;

  // 4.2.2 Basic Sequences for Object Control in General
  // (B) Processing when the controlled object exists,
  // except when ESV = 0x60 to 0x63, 0x6E and 0x74

  esv = uecho_message_getesv(msg);
  if (!uecho_message_isrequestesv(esv))
    return;

  msgOpc = uecho_message_getopc(msg);
  for (n = 0; n < msgOpc; n++) {
    msgProp = uecho_message_getproperty(msg, n);
    if (!msgProp)
      continue;

    // 4.2.2 Basic Sequences for Object Control in General
    // (C) Processing when the controlled property exists but
    // the controlled property doesn’t exist or can be processed only partially

    nodeProp = uecho_object_getproperty(msgDestObj, uecho_property_getcode(msgProp));
    if (!nodeProp) {
      uecho_object_responsemessage(msgDestObj, msg);
      return;
    }

    // 4.2.2 Basic Sequences for Object Control in General
    // (D) Processing when the controlled property exists but
    // the stipulated service (ESV=0x60-0x63, 0x6E) processing functions are not available

    if (!uecho_property_isacceptablemessage(nodeProp, msg)) {
      uecho_object_responsemessage(msgDestObj, msg);
      return;
    }

    // 4.2.2 Basic Sequences for Object Control in General
    // (E) Processing when the controlled property exists and the stipulated service
    // (ESV=0x60, 0x61, 0x6E) processing functions are available but the EDT size does not match

    if (uecho_message_iswriterequest(msg)) {
      if (uecho_property_getdatasize(msgProp) != uecho_property_getdatasize(nodeProp)) {
        uecho_object_responsemessage(msgDestObj, msg);
        return;
      }
    }
  }

  uecho_object_handlemessage(msgDestObj, msg);
}
