/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_node.h>

#include <uecho/frame/observer.h>
#include <uecho/frame/server.h>

/****************************************
 * uecho_node_isselfobjectmessage
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
 * uecho_object_notifyrequestproperty
 ****************************************/

bool uecho_object_notifyrequestproperty(uEchoObject* obj, uEchoProperty* objProp, uEchoEsv msgEsv, uEchoProperty* msgProp)
{
  if (!obj || !msgProp)
    return false;

  bool areAllHandlerAccepted = true;

  for (uEchoObjectPropertyObserver* obs = uecho_object_property_observer_manager_getobservers(obj->propListenerMgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (msgEsv != uecho_object_property_observer_getesv(obs))
      continue;
    if (uecho_property_getcode(msgProp) != uecho_object_property_observer_getpropetycode(obs))
      continue;
    areAllHandlerAccepted &= obs->handler(obj, objProp, msgEsv, uecho_property_getdatasize(msgProp), uecho_property_getdata(msgProp));
  }

  return areAllHandlerAccepted;
}

/****************************************
 * uecho_node_servermessagelistener
 ****************************************/

typedef bool (*uEchoMessageAddPropertyFunc)(uEchoMessage* msg, uEchoProperty* prop);

bool uecho_node_handlerequestmessage(uEchoObject* destObj, uEchoEsv msgEsv, byte opc, uEchoProperty** ep, uEchoMessageAddPropertyFunc messageAddpropertyFunc, uEchoMessage* resMsg)
{
  uEchoPropertyCode msgPropCode;
  uEchoProperty *msgProp, *destProp, *resProp;
  int acceptedRequestCnt, n;

  acceptedRequestCnt = 0;
  for (n = 0; n < opc; n++) {
    msgProp = ep[n];
    if (!msgProp)
      continue;
    msgPropCode = uecho_property_getcode(msgProp);

    resProp = uecho_property_new();
    if (!resProp)
      continue;
    uecho_property_setcode(resProp, msgPropCode);

    destProp = uecho_object_getproperty(destObj, msgPropCode);
    if (destProp) {
      if (uecho_object_notifyrequestproperty(destObj, destProp, msgEsv, msgProp)) {
        acceptedRequestCnt++;
        switch (msgEsv) {
        case uEchoEsvWriteRequest:
        case uEchoEsvWriteRequestResponseRequired:
          uecho_property_setdata(destProp, uecho_property_getdata(msgProp), uecho_property_getdatasize(msgProp));
          break;
        }
        switch (msgEsv) {
        case uEchoEsvReadRequest:
        case uEchoEsvNotificationRequest:
        case uEchoEsvNotificationResponseRequired:
          uecho_property_setdata(resProp, uecho_property_getdata(destProp), uecho_property_getdatasize(destProp));
          break;
        }
      }
      else {
        switch (msgEsv) {
        case uEchoEsvWriteRequestResponseRequired:
          uecho_property_setdata(resProp, uecho_property_getdata(msgProp), uecho_property_getdatasize(msgProp));
          break;
        }
      }
    }
    else {
      switch (msgEsv) {
      case uEchoEsvWriteRequestResponseRequired:
        uecho_property_setdata(resProp, uecho_property_getdata(msgProp), uecho_property_getdatasize(msgProp));
        break;
      }
    }

    messageAddpropertyFunc(resMsg, resProp);
  }

  return acceptedRequestCnt;
}

void uecho_node_servermessagelistener(uEchoNode* node, uEchoMessage* reqMsg)
{
  uEchoEsv reqEsv, resEsv;
  uEchoObjectCode msgDstObjCode;
  uEchoObject* msgDestObj;
  int allRequestCnt, acceptedRequestCnt;
  uEchoMessage* resMsg;
  bool isResponseRequired;
  byte* resMsgBytes;
  size_t resMsgLen;
  uEchoNode* parentNode;

  if (!node || !reqMsg)
    return;

  if (node->msgListener) {
    node->msgListener(node, reqMsg);
  }

  // 4.2.2 Basic Sequences for Object Control in General
  // (A) | Processing when the controlled object does not exist

  msgDstObjCode = uecho_message_getdestinationobjectcode(reqMsg);
  msgDestObj = uecho_node_getobjectbycode(node, msgDstObjCode);

  if (!msgDestObj)
    return;

  if (msgDestObj->allMsgListener) {
    msgDestObj->allMsgListener(msgDestObj, reqMsg);
  }

  // 4.2.2 Basic Sequences for Object Control in General
  // (B) Processing when the controlled object exists,
  // except when ESV = 0x60 to 0x63, 0x6E and 0x74

  reqEsv = uecho_message_getesv(reqMsg);
  if (!uecho_esv_isrequest(reqEsv))
    return;

  resMsg = uecho_message_new();
  if (!resMsg)
    return;
  uecho_message_settid(resMsg, uecho_message_gettid(reqMsg));
  uecho_message_setsourceobjectcode(resMsg, uecho_message_getdestinationobjectcode(reqMsg));
  uecho_message_setdestinationobjectcode(resMsg, uecho_message_getsourceobjectcode(reqMsg));

  acceptedRequestCnt = 0;
  if (uecho_message_isreadwritemessage(reqMsg)) {
    acceptedRequestCnt += uecho_node_handlerequestmessage(msgDestObj, uEchoEsvWriteRequestResponseRequired, reqMsg->opcSet, reqMsg->epSet, uecho_message_addpropertyset, resMsg);
    acceptedRequestCnt += uecho_node_handlerequestmessage(msgDestObj, uEchoEsvReadRequest, reqMsg->opcGet, reqMsg->epGet, uecho_message_addpropertyget, resMsg);
  }
  else {
    acceptedRequestCnt += uecho_node_handlerequestmessage(msgDestObj, reqEsv, reqMsg->opc, reqMsg->ep, uecho_message_addproperty, resMsg);
  }

  allRequestCnt = reqMsg->opc + reqMsg->opcSet + reqMsg->opcGet;
  isResponseRequired = true;

  resEsv = reqEsv;
  switch (reqEsv) {
  // 4.2.3.1 Property value write service (no response required) [0x60, 0x50]
  case uEchoEsvWriteRequest: {
    if (acceptedRequestCnt == allRequestCnt)
      isResponseRequired = false;
    else
      resEsv = uEchoEsvWriteRequestError;

  } break;
  // 4.2.3.2 Property value write service (response required) [0x61,0x71,0x51]
  case uEchoEsvWriteRequestResponseRequired: {
    if (acceptedRequestCnt == allRequestCnt)
      resEsv = uEchoEsvWriteResponse;
    else
      resEsv = uEchoEsvWriteRequestError;

  } break;
    // 4.2.3.3 Property value read service [0x62,0x72,0x52]
  case uEchoEsvReadRequest: {
    if (acceptedRequestCnt == allRequestCnt)
      resEsv = uEchoEsvReadResponse;
    else
      resEsv = uEchoEsvReadRequestError;
  } break;
  // 4.2.3.4 Property value write & read service [0x6E,0x7E,0x5E]
  case uEchoEsvWriteReadRequest: {
    if (acceptedRequestCnt == allRequestCnt)
      resEsv = uEchoEsvWriteReadResponse;
    else
      resEsv = uEchoEsvWriteReadRequestError;
  } break;
  // 4.2.3.5 Property value notification service [0x63,0x73,0x53]
  case uEchoEsvNotificationRequest: {
    if (acceptedRequestCnt == allRequestCnt)
      resEsv = uEchoEsvNotification;
    else
      resEsv = uEchoEsvNotificationRequestError;
  } break;
    // 4.2.3.6 Property value notification service (response required) [0x74, 0x7A]
  case uEchoEsvNotificationResponseRequired: {
    if (acceptedRequestCnt == allRequestCnt)
      resEsv = uEchoEsvNotificationResponse;
    else
      isResponseRequired = false;
  } break;
  default: {
    isResponseRequired = false;
  } break;
  }

  parentNode = uecho_object_getparentnode(msgDestObj);
  if (isResponseRequired && parentNode) {
    uecho_message_setesv(resMsg, resEsv);
    resMsgBytes = uecho_message_getbytes(resMsg);
    resMsgLen = uecho_message_size(resMsg);
    if (uecho_message_getesv(resMsg) == uEchoEsvNotification) {
      uecho_node_announcemessagebytes(parentNode, resMsgBytes, resMsgLen);
    }
    else {
      uecho_node_sendmessagebytes(parentNode, uecho_message_getsourceaddress(reqMsg), resMsgBytes, resMsgLen);
    }
  }

  uecho_message_delete(resMsg);
}
