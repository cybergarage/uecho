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
* uecho_node_servermessagelistener
****************************************/

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
        if (uecho_esv_isreadrequest(reqEsv) || uecho_esv_isnotifyrequest(reqEsv)) {
          uecho_property_setdata(resProp, uecho_property_getdata(objProp), uecho_property_getdatasize(objProp));
        }
      }
      else {
        if (uecho_esv_iswriterequest(reqEsv)) {
          uecho_property_setdata(resProp, uecho_property_getdata(msgProp), uecho_property_getdatasize(msgProp));
        }
      }
    }

    uecho_message_addproperty(resMsg, resProp);
  }

  return accepted_request_cnt;
}

void uecho_node_servermessagelistener(uEchoServer* server, uEchoMessage* reqMsg)
{
  uEchoEsv reqEsv, resEsv;
  uEchoNode* node;
  uEchoObjectCode msgDstObjCode;
  uEchoObject* msgDestObj;
  int all_request_cnt, accepted_request_cnt;
  uEchoMessage* resMsg;
  bool is_response_required;
  byte* resMsgBytes;
  size_t resMsgLen;
  uEchoNode* parentNode;

  if (!server || !reqMsg)
    return;

  node = (uEchoNode*)uecho_server_getuserdata(server);
  if (!node)
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

  accepted_request_cnt = 0;
  if (uecho_message_iswriterequest(reqMsg)) {
    accepted_request_cnt += uecho_node_handlerequestmessage(msgDestObj, uEchoEsvWriteRequest, reqMsg->OPCSet, reqMsg->EPSet, resMsg);
    accepted_request_cnt += uecho_node_handlerequestmessage(msgDestObj, uEchoEsvReadRequest, reqMsg->OPCGet, reqMsg->EPGet, resMsg);
  }
  else {
    accepted_request_cnt += uecho_node_handlerequestmessage(msgDestObj, reqEsv, reqMsg->OPC, reqMsg->EP, resMsg);
  }

  all_request_cnt = reqMsg->OPC + reqMsg->OPCSet + reqMsg->OPCGet;
  is_response_required = true;

  resEsv = reqEsv;
  switch (reqEsv) {
  // 4.2.3.1 Property value write service (no response required) [0x60, 0x50]
  case uEchoEsvWriteRequest: {
    if (accepted_request_cnt == all_request_cnt)
      is_response_required = false;
    else
      resEsv = uEchoEsvWriteRequestError;

  } break;
  // 4.2.3.2 Property value write service (response required) [0x61,0x71,0x51]
  case uEchoEsvWriteRequestResponseRequired: {
    if (accepted_request_cnt == all_request_cnt)
      resEsv = uEchoEsvWriteResponse;
    else
      resEsv = uEchoEsvWriteRequestError;

  } break;
    // 4.2.3.3 Property value read service [0x62,0x72,0x52]
  case uEchoEsvReadRequest: {
    if (accepted_request_cnt == all_request_cnt)
      resEsv = uEchoEsvReadResponse;
    else
      resEsv = uEchoEsvReadRequestError;
  } break;
  // 4.2.3.4 Property value write & read service [0x6E,0x7E,0x5E]
  case uEchoEsvWriteReadRequest: {
    if (accepted_request_cnt == all_request_cnt)
      resEsv = uEchoEsvWriteReadResponse;
    else
      resEsv = uEchoEsvWriteReadRequestError;
  } break;
  // 4.2.3.5 Property value notification service [0x63,0x73,0x53]
  case uEchoEsvNotificationRequest: {
    if (accepted_request_cnt == all_request_cnt)
      resEsv = uEchoEsvNotification;
    else
      resEsv = uEchoEsvNotificationRequestError;
  } break;
    // 4.2.3.6 Property value notification service (response required) [0x74, 0x7A]
  case uEchoEsvNotificationResponseRequired: {
    if (accepted_request_cnt == all_request_cnt)
      resEsv = uEchoEsvNotificationResponse;
    else
      is_response_required = false;
  } break;
  default: {
    is_response_required = false;
  } break;
  }

  parentNode = uecho_object_getparentnode(msgDestObj);
  if (is_response_required && parentNode) {
    resMsgBytes = uecho_message_getbytes(resMsg);
    resMsgLen = uecho_message_size(resMsg);
    uecho_message_setesv(resMsg, resEsv);
    if (uecho_message_getesv(resMsg) == uEchoEsvNotification) {
      uecho_node_announcemessagebytes(parentNode, resMsgBytes, resMsgLen);
    }
    else {
      uecho_node_sendmessagebytes(parentNode, uecho_message_getsourceaddress(reqMsg), resMsgBytes, resMsgLen);
    }
  }

  uecho_message_delete(resMsg);
}
