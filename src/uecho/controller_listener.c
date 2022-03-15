/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller_internal.h>

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

  node = uecho_controller_getnodebyaddress(ctrl, msg_addr);
  if (!node) {
    node = uecho_node_new();
    if (!node)
      return;
    uecho_node_setaddress(node, uecho_message_getsourceaddress(msg));
    uecho_controller_addnode(ctrl, node);
  }

  // Updated node

  prop_data = uecho_property_getdata(prop);

  printf("%s\n", msg_addr);
  for (idx = 1; (idx + 2) < prop_size; idx += 3) {
    obj_code = uecho_byte2integer((prop_data + idx), 3);
    printf("%X\n", obj_code);
    uecho_node_setobject(node, obj_code);
  }
}

/****************************************
 * uecho_controller_updatepropertydata
 ****************************************/

void uecho_controller_updateopcpropertydata(uEchoController* ctrl, uEchoObject* src_obj, byte opc, uEchoProperty** ep)
{
  uEchoProperty* msg_prop;
  uEchoPropertyCode msg_prop_code;
  size_t n;

  for (n = 0; n < opc; n++) {
    msg_prop = ep[n];
    if (!msg_prop)
      continue;
    msg_prop_code = uecho_property_getcode(msg_prop);
    if (!uecho_object_hasproperty(src_obj, msg_prop_code)) {
      uecho_object_setproperty(src_obj, msg_prop_code, uEchoPropertyAttrNone);
    }
    uecho_object_setpropertydata(src_obj, msg_prop_code, uecho_property_getdata(msg_prop), uecho_property_getdatasize(msg_prop));
  }
}

void uecho_controller_updatepropertydata(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoNode* src_node;
  uEchoObject* src_obj;

  src_node = uecho_controller_getnodebyaddress(ctrl, uecho_message_getsourceaddress(msg));
  if (!src_node)
    return;

  src_obj = uecho_node_getobjectbycode(src_node, uecho_message_getsourceobjectcode(msg));
  if (!src_obj)
    return;

  if (uecho_message_isreadwritemessage(msg)) {
    uecho_controller_updateopcpropertydata(ctrl, src_obj, msg->opc_get, msg->ep_get);
  }
  else {
    uecho_controller_updateopcpropertydata(ctrl, src_obj, msg->opc, msg->ep);
  }
}

/****************************************
 * uecho_controller_handlepostresponse
 ****************************************/

void uecho_controller_handlepostresponse(uEchoController* ctrl, uEchoMessage* msg)
{
  if (!uecho_controller_ispostresponsemessage(ctrl, msg))
    return;

  uecho_message_set(uecho_controller_getpostresponsemessage(ctrl), msg);
}

/****************************************
 * uecho_controller_handlerequestmessage
 ****************************************/

void uecho_controller_handlerequestmessage(uEchoController* ctrl, uEchoMessage* msg)
{
  if (uecho_controller_ispostresponsewaiting(ctrl)) {
    uecho_controller_handlepostresponse(ctrl, msg);
  }

  if (uecho_message_issearchresponse(msg)) {
    uecho_controller_handlesearchmessage(ctrl, msg);
    return;
  }

  if (uecho_message_isreadresponse(msg) || uecho_message_isnotifyresponse(msg)) {
    uecho_controller_updatepropertydata(ctrl, msg);
    return;
  }
}

/****************************************
* uecho_controller_servermessagelistener
****************************************/

void uecho_controller_servermessagelistener(uEchoServer* server, uEchoMessage* msg)
{
  uEchoController* ctrl;

  if (!server || !msg)
    return;

  ctrl = (uEchoController*)uecho_server_getuserdata(server);
  if (!ctrl)
    return;

  if (!uecho_node_hasobjectbycode(ctrl->node, uecho_message_getdestinationobjectcode(msg))) {
    if (ctrl->msg_listener) {
      ctrl->msg_listener(ctrl, msg);
    }
    return;
  }

  uecho_controller_handlerequestmessage(ctrl, msg);

  if (ctrl->msg_listener) {
    ctrl->msg_listener(ctrl, msg);
  }
}
