/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_controller.h>

#include <uecho/misc.h>
#include <uecho/profile.h>

/****************************************
 * uecho_controller_handlesearchmessage
 ****************************************/

void uecho_controller_handlesearchmessage(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoNode* node;
  uEchoProperty* prop;
  uEchoObjectCode obj_code;
  byte* prop_data;
  size_t prop_size;
  size_t idx;
  const char* msg_addr;
  bool node_added;
  bool node_updated;

  // Check message

  prop = uecho_message_getpropertybycode(msg, uEchoNodeProfileClassSelfNodeInstanceListS);
  if (!prop)
    return;

  prop_size = uecho_property_getdatasize(prop);
  if (prop_size < 1)
    return;

  // Get or create node

  msg_addr = uecho_message_getsourceaddress(msg);
  if (!msg_addr)
    return;

  node_added = false;
  node = uecho_controller_getnodebyaddress(ctrl, msg_addr);
  if (!node) {
    node = uecho_node_new();
    if (!node)
      return;
    uecho_node_setaddress(node, uecho_message_getsourceaddress(msg));
    uecho_controller_addnode(ctrl, node);
    node_added = true;
  }

  // Updated node

  node_updated = false;
  prop_data = uecho_property_getdata(prop);
  for (idx = 1; (idx + 2) < prop_size; idx += 3) {
    obj_code = uecho_byte2integer((prop_data + idx), 3);
    if (uecho_node_hasobjectbycode(node, obj_code))
      continue;
    uecho_node_setobject(node, obj_code);
    node_updated = true;
  }

  // Notify node status

  if (ctrl->node_listener) {
    if (node_added) {
      ctrl->node_listener(ctrl, node, uEchoNodeStatusAdded, msg);
    }
    else if (node_updated) {
      ctrl->node_listener(ctrl, node, uEchoNodeStatusUpdated, msg);
    }
  }
}

/****************************************
 * uecho_controller_updatepropertydata
 ****************************************/

bool uecho_controller_updateopcpropertydata(uEchoController* ctrl, uEchoObject* obj, byte opc, uEchoProperty** ep)
{
  uEchoProperty* msg_prop;
  uEchoPropertyCode msg_prop_code;
  byte* msg_data;
  size_t msg_data_size;
  uEchoProperty* obj_prop;
  bool obj_prop_updated;
  size_t n;

  obj_prop_updated = false;
  for (n = 0; n < opc; n++) {
    msg_prop = ep[n];
    if (!msg_prop)
      continue;

    msg_prop_code = uecho_property_getcode(msg_prop);
    if (!uecho_object_hasproperty(obj, msg_prop_code)) {
      uecho_object_setproperty(obj, msg_prop_code, uEchoPropertyAttrNone);
    }

    obj_prop = uecho_object_getproperty(obj, msg_prop_code);
    if (!obj_prop)
      continue;

    msg_data = uecho_property_getdata(msg_prop);
    msg_data_size = uecho_property_getdatasize(msg_prop);
    if (uecho_property_isdataequal(obj_prop, msg_data, msg_data_size))
      continue;

    uecho_property_setdata(obj_prop, msg_data, msg_data_size);

    obj_prop_updated = true;
  }

  return obj_prop_updated;
}

bool uecho_controller_updatenodebyresponsemessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg)
{
  uEchoObject* node_obj;
  bool node_updated;

  node_obj = uecho_node_getobjectbycode(node, uecho_message_getsourceobjectcode(msg));
  if (!node_obj)
    return false;

  node_updated = false;
  if (uecho_message_isreadwritemessage(msg)) {
    node_updated = uecho_controller_updateopcpropertydata(ctrl, node_obj, msg->opc_get, msg->ep_get);
  }
  else {
    node_updated = uecho_controller_updateopcpropertydata(ctrl, node_obj, msg->opc, msg->ep);
  }

  return node_updated;
}

/****************************************
 * uecho_controller_handlepostresponse
 ****************************************/

void uecho_controller_handlepostresponse(uEchoController* ctrl, uEchoMessage* msg)
{
  if (!uecho_controller_ispostresponsemessage(ctrl, msg))
    return;

  uecho_message_set(uecho_controller_getpostresponsemessage(ctrl), msg);
  uecho_cond_signal(ctrl->cond);
}

/****************************************
 * uecho_controller_handlenodemessage
 ****************************************/

void uecho_controller_handlenodemessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg)
{
  bool node_updated;

  node_updated = false;
  if (uecho_message_isnotification(msg) || uecho_message_isreadresponse(msg) || uecho_message_isnotifyresponse(msg)) {
    node_updated = uecho_controller_updatenodebyresponsemessage(ctrl, node, msg);
  }

  // Notify node status

  if (ctrl->node_listener) {
    if (uecho_message_isnotification(msg)) {
      ctrl->node_listener(ctrl, node, uEchoNodeStatusAnnounced, msg);
    }
    if (uecho_message_isresponse(msg)) {
      ctrl->node_listener(ctrl, node, uEchoNodeStatusResponded, msg);
    }
    if (node_updated) {
      ctrl->node_listener(ctrl, node, uEchoNodeStatusUpdated, msg);
    }
  }
}

/****************************************
 * uecho_controller_servermessagelistener
 ****************************************/

void uecho_controller_servermessagelistener(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoNode* src_node;

  if (!ctrl || !msg)
    return;

  if (ctrl->msg_listener) {
    ctrl->msg_listener(ctrl, msg);
  }

  if (uecho_controller_ispostresponsewaiting(ctrl)) {
    uecho_controller_handlepostresponse(ctrl, msg);
  }

  if (uecho_message_issearchresponse(msg)) {
    uecho_controller_handlesearchmessage(ctrl, msg);
    return;
  }

  if (uecho_node_hasobjectbycode(ctrl->node, uecho_message_getdestinationobjectcode(msg))) {
    src_node = uecho_controller_getnodebyaddress(ctrl, uecho_message_getsourceaddress(msg));
    if (src_node) {
      uecho_controller_handlenodemessage(ctrl, src_node, msg);
    }
  }
}
