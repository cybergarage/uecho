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

bool uecho_object_notifyrequestproperty(uEchoObject* obj, uEchoEsv esv, uEchoProperty* msgProp)
{
  uEchoObjectPropertyObserver* obs;
  bool are_all_handler_accepted = true;

  if (!obj || !msgProp)
    return false;

  for (obs = uecho_object_property_observer_manager_getobservers(obj->propListenerMgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (esv != uecho_object_property_observer_getesv(obs))
      continue;
    if (uecho_property_getcode(msgProp) != uecho_object_property_observer_getpropetycode(obs))
      continue;
    are_all_handler_accepted &= obs->handler(obj, esv, msgProp);
  }

  return are_all_handler_accepted;
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

void uecho_object_responsemessage_setopc(uEchoMessage* resMsg, uEchoObject* obj, byte OPC, uEchoProperty** EP)
{
  uEchoProperty *msgProp, *nodeProp, *resProp;
  uEchoPropertyCode msgPropCode;
  int nodePropSize;
  byte* nodePropData;
  int n;

  for (n = 0; n < OPC; n++) {
    msgProp = EP[n];
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
}

uEchoMessage* uecho_object_createresponsemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoEsv msgEsv, resEsv;
  uEchoMessage* resMsg;

  if (!obj || !msg)
    return NULL;

  msgEsv = uecho_message_getesv(msg);
  if (!uecho_message_requestesv2responseesv(msgEsv, &resEsv))
    return NULL;

  resMsg = uecho_message_new();
  if (!resMsg)
    return NULL;

  uecho_message_setesv(resMsg, resEsv);
  uecho_message_settid(resMsg, uecho_message_gettid(msg));
  uecho_message_setsourceobjectcode(resMsg, uecho_message_getdestinationobjectcode(msg));
  uecho_message_setdestinationobjectcode(resMsg, uecho_message_getsourceobjectcode(msg));

  if (uecho_message_isreadwritemessage(msg)) {
    uecho_object_responsemessage_setopc(resMsg, obj, msg->OPCSet, msg->EPSet);
    uecho_object_responsemessage_setopc(resMsg, obj, msg->OPCGet, msg->EPGet);
  }
  else {
    uecho_object_responsemessage_setopc(resMsg, obj, msg->OPC, msg->EP);
  }

  return resMsg;
}

bool uecho_object_responsemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoNode* parentNode;
  uEchoMessage* resMsg;
  byte* resMsgBytes;
  size_t resMsgLen;

  if (!obj || !msg)
    return false;

  parentNode = uecho_object_getparentnode(obj);
  if (!parentNode)
    return false;

  // Create response message

  resMsg = uecho_object_createresponsemessage(obj, msg);
  if (!resMsg)
    return false;

  // Send response message

  resMsgBytes = uecho_message_getbytes(resMsg);
  resMsgLen = uecho_message_size(resMsg);
  if (uecho_message_getesv(resMsg) == uEchoEsvNotification) {
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

void uecho_object_handlewritemessage(uEchoObject* obj, byte OPC, uEchoProperty** EP)
{
  uEchoProperty* msgProp;
  int n;
  for (n = 0; n < OPC; n++) {
    msgProp = EP[n];
    if (!msgProp)
      continue;
    if (!uecho_object_iswritablepropertyrequest(obj, msgProp))
      continue;
    uecho_object_setpropertyrequest(obj, msgProp);
  }
}

int uecho_object_handlerequestmessage(uEchoObject* obj, uEchoEsv msgEsv, byte OPC, uEchoProperty** EP)
{
  uEchoProperty* msgProp;
  int accepted_request_cnt, n;

  accepted_request_cnt = 0;
  for (n = 0; n < OPC; n++) {
    msgProp = EP[n];
    if (!msgProp)
      continue;
    if (uecho_object_notifyrequestproperty(obj, msgEsv, msgProp))
      accepted_request_cnt++;
  }

  return accepted_request_cnt;
}

void uecho_object_handlemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoEsv msgEsv;

  if (!obj || !msg)
    return;

  msgEsv = uecho_message_getesv(msg);

  // Write properties

  if (uecho_message_iswriterequest(msg)) {
    uecho_object_handlewritemessage(obj, msg->OPC, msg->EP);
    uecho_object_handlewritemessage(obj, msg->OPCSet, msg->EPSet);
  }

  // Notify observers

  if (obj->allMsgListener) {
    obj->allMsgListener(obj, msg);
  }

  // Notify request observers

  uecho_object_handlerequestmessage(obj, msgEsv, msg->OPC, msg->EP);
  uecho_object_handlerequestmessage(obj, uEchoEsvWriteRequest, msg->OPCSet, msg->EPSet);
  uecho_object_handlerequestmessage(obj, uEchoEsvReadRequest, msg->OPCGet, msg->EPGet);

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

int uecho_node_handlerequestmessage(uEchoObject* destObj, uEchoEsv reqEsv, byte OPC, uEchoProperty** EP, uEchoMessage* resMsg)
{
  uEchoPropertyCode msgPropCode;
  uEchoProperty *msgProp, *objProp, *resProp;
  int accepted_request_cnt, n;

  accepted_request_cnt = 0;
  for (n = 0; n < OPC; n++) {
    msgProp = EP[n];
    if (!msgProp)
      continue;
    msgPropCode = uecho_property_getcode(msgProp);

    resProp = uecho_property_new();
    if (!resProp)
      continue;
    uecho_property_setcode(resProp, msgPropCode);

    objProp = uecho_object_getproperty(destObj, msgPropCode);
    if (objProp) {
      if (uecho_object_notifyrequestproperty(destObj, reqEsv, objProp)) {
        accepted_request_cnt++;
        if (uecho_esv_isreadrequest(reqEsv)) {
        }
      }
      else {
        if (uecho_esv_iswriterequest(reqEsv)) {
        }
      }
    }

    uecho_message_addproperty(resMsg, resProp);
  }

  return accepted_request_cnt;
}

void uecho_node_servermessagelistener(uEchoServer* server, uEchoMessage* msg)
{
  uEchoEsv esv;
  uEchoNode* node;
  uEchoObjectCode msgDstObjCode;
  uEchoObject* msgDestObj;
  int msgOpc, n;
  uEchoProperty *msgProp, *nodeProp;
  int accepted_request_cnt;
  uEchoMessage* resMsg;

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

  if (msgDestObj->allMsgListener) {
    msgDestObj->allMsgListener(msgDestObj, msg);
  }

  // 4.2.2 Basic Sequences for Object Control in General
  // (B) Processing when the controlled object exists,
  // except when ESV = 0x60 to 0x63, 0x6E and 0x74

  esv = uecho_message_getesv(msg);
  if (!uecho_message_isrequestesv(esv))
    return;

  resMsg = uecho_message_new();
  if (!resMsg)
    return;
  uecho_message_settid(resMsg, uecho_message_gettid(msg));
  uecho_message_setsourceobjectcode(resMsg, uecho_message_getdestinationobjectcode(msg));
  uecho_message_setdestinationobjectcode(resMsg, uecho_message_getsourceobjectcode(msg));

  accepted_request_cnt = 0;
  if (uecho_message_iswriterequest(msg)) {
    accepted_request_cnt += uecho_node_handlerequestmessage(msgDestObj, uEchoEsvWriteRequest, msg->OPCSet, msg->EPSet, resMsg);
    accepted_request_cnt += uecho_node_handlerequestmessage(msgDestObj, uEchoEsvReadRequest, msg->OPCGet, msg->EPGet, resMsg);
  }
  else {
    accepted_request_cnt += uecho_node_handlerequestmessage(msgDestObj, esv, msg->OPC, msg->EP, resMsg);
  }

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
