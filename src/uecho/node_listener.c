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

bool uecho_object_notifyrequestproperty(uEchoObject* obj, uEchoProperty* obj_prop, uEchoEsv msg_esv, uEchoProperty* msg_prop)
{
  if (!obj || !msg_prop)
    return false;

  bool are_all_handler_accepted = true;

  for (uEchoObjectPropertyObserver *obs = uecho_object_property_observer_manager_getobservers(obj->prop_listener_mgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (msg_esv != uecho_object_property_observer_getesv(obs))
      continue;
    if (uecho_property_getcode(msg_prop) != uecho_object_property_observer_getpropetycode(obs))
      continue;
    are_all_handler_accepted &= obs->handler(obj, msg_esv, msg_prop);
  }

  return are_all_handler_accepted;
}

/****************************************
* uecho_node_servermessagelistener
****************************************/

int uecho_node_handlerequestmessage(uEchoObject* dest_obj, uEchoEsv msg_esv, byte opc, uEchoProperty** ep, uEchoMessage* res_msg)
{
  uEchoPropertyCode msg_prop_code;
  uEchoProperty *msg_prop, *dest_prop, *res_prop;
  int accepted_request_cnt, n;

  accepted_request_cnt = 0;
  for (n = 0; n < opc; n++) {
    msg_prop = ep[n];
    if (!msg_prop)
      continue;
    msg_prop_code = uecho_property_getcode(msg_prop);

    res_prop = uecho_property_new();
    if (!res_prop)
      continue;
    uecho_property_setcode(res_prop, msg_prop_code);

    dest_prop = uecho_object_getproperty(dest_obj, msg_prop_code);
    if (dest_prop) {
      if (uecho_object_notifyrequestproperty(dest_obj, dest_prop, msg_esv, msg_prop)) {
        accepted_request_cnt++;
        if (uecho_esv_isreadrequest(msg_esv) || uecho_esv_isnotifyrequest(msg_esv)) {
          uecho_property_setdata(res_prop, uecho_property_getdata(dest_prop), uecho_property_getdatasize(dest_prop));
        }
      }
      else {
        if (uecho_esv_iswriterequest(msg_esv)) {
          uecho_property_setdata(res_prop, uecho_property_getdata(msg_prop), uecho_property_getdatasize(msg_prop));
        }
      }
    }

    uecho_message_addproperty(res_msg, res_prop);
  }

  return accepted_request_cnt;
}

void uecho_node_servermessagelistener(uEchoServer* server, uEchoMessage* req_msg)
{
  uEchoEsv req_esv, res_esv;
  uEchoNode* node;
  uEchoObjectCode msg_dst_obj_code;
  uEchoObject* msg_dest_obj;
  int all_request_cnt, accepted_request_cnt;
  uEchoMessage* res_msg;
  bool is_response_required;
  byte* res_msg_bytes;
  size_t res_msg_len;
  uEchoNode* parent_node;

  if (!server || !req_msg)
    return;

  node = (uEchoNode*)uecho_server_getuserdata(server);
  if (!node)
    return;

  if (node->msg_listener) {
    node->msg_listener(node, req_msg);
  }

  // 4.2.2 Basic Sequences for Object Control in General
  // (A) | Processing when the controlled object does not exist

  msg_dst_obj_code = uecho_message_getdestinationobjectcode(req_msg);
  msg_dest_obj = uecho_node_getobjectbycode(node, msg_dst_obj_code);

  if (!msg_dest_obj)
    return;

  if (msg_dest_obj->all_msg_listener) {
    msg_dest_obj->all_msg_listener(msg_dest_obj, req_msg);
  }

  // 4.2.2 Basic Sequences for Object Control in General
  // (B) Processing when the controlled object exists,
  // except when ESV = 0x60 to 0x63, 0x6E and 0x74

  req_esv = uecho_message_getesv(req_msg);
  if (!uecho_esv_isrequest(req_esv))
    return;

  res_msg = uecho_message_new();
  if (!res_msg)
    return;
  uecho_message_settid(res_msg, uecho_message_gettid(req_msg));
  uecho_message_setsourceobjectcode(res_msg, uecho_message_getdestinationobjectcode(req_msg));
  uecho_message_setdestinationobjectcode(res_msg, uecho_message_getsourceobjectcode(req_msg));

  accepted_request_cnt = 0;
  if (uecho_message_isreadwritemessage(req_msg)) {
    accepted_request_cnt += uecho_node_handlerequestmessage(msg_dest_obj, uEchoEsvWriteRequest, req_msg->opc_set, req_msg->ep_set, res_msg);
    accepted_request_cnt += uecho_node_handlerequestmessage(msg_dest_obj, uEchoEsvReadRequest, req_msg->opc_get, req_msg->ep_get, res_msg);
  }
  else {
    accepted_request_cnt += uecho_node_handlerequestmessage(msg_dest_obj, req_esv, req_msg->opc, req_msg->ep, res_msg);
  }

  all_request_cnt = req_msg->opc + req_msg->opc_set + req_msg->opc_get;
  is_response_required = true;

  res_esv = req_esv;
  switch (req_esv) {
  // 4.2.3.1 Property value write service (no response required) [0x60, 0x50]
  case uEchoEsvWriteRequest: {
    if (accepted_request_cnt == all_request_cnt)
      is_response_required = false;
    else
      res_esv = uEchoEsvWriteRequestError;

  } break;
  // 4.2.3.2 Property value write service (response required) [0x61,0x71,0x51]
  case uEchoEsvWriteRequestResponseRequired: {
    if (accepted_request_cnt == all_request_cnt)
      res_esv = uEchoEsvWriteResponse;
    else
      res_esv = uEchoEsvWriteRequestError;

  } break;
    // 4.2.3.3 Property value read service [0x62,0x72,0x52]
  case uEchoEsvReadRequest: {
    if (accepted_request_cnt == all_request_cnt)
      res_esv = uEchoEsvReadResponse;
    else
      res_esv = uEchoEsvReadRequestError;
  } break;
  // 4.2.3.4 Property value write & read service [0x6E,0x7E,0x5E]
  case uEchoEsvWriteReadRequest: {
    if (accepted_request_cnt == all_request_cnt)
      res_esv = uEchoEsvWriteReadResponse;
    else
      res_esv = uEchoEsvWriteReadRequestError;
  } break;
  // 4.2.3.5 Property value notification service [0x63,0x73,0x53]
  case uEchoEsvNotificationRequest: {
    if (accepted_request_cnt == all_request_cnt)
      res_esv = uEchoEsvNotification;
    else
      res_esv = uEchoEsvNotificationRequestError;
  } break;
    // 4.2.3.6 Property value notification service (response required) [0x74, 0x7A]
  case uEchoEsvNotificationResponseRequired: {
    if (accepted_request_cnt == all_request_cnt)
      res_esv = uEchoEsvNotificationResponse;
    else
      is_response_required = false;
  } break;
  default: {
    is_response_required = false;
  } break;
  }

  parent_node = uecho_object_getparentnode(msg_dest_obj);
  if (is_response_required && parent_node) {
    uecho_message_setesv(res_msg, res_esv);
    res_msg_bytes = uecho_message_getbytes(res_msg);
    res_msg_len = uecho_message_size(res_msg);
    if (uecho_message_getesv(res_msg) == uEchoEsvNotification) {
      uecho_node_announcemessagebytes(parent_node, res_msg_bytes, res_msg_len);
    }
    else {
      uecho_node_sendmessagebytes(parent_node, uecho_message_getsourceaddress(req_msg), res_msg_bytes, res_msg_len);
    }
  }

  uecho_message_delete(res_msg);
}
