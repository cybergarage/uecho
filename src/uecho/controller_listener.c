/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
  uEchoObjectCode objCode;
  byte* propData;
  size_t propSize;
  size_t idx;
  const char* msgAddr;

  // Check message

  prop = uecho_message_getpropertybycode(msg, uEchoNodeProfileClassSelfNodeInstanceListS);
  if (!prop)
    return;

  propSize = uecho_property_getdatasize(prop);
  if (propSize < 1)
    return;

  // Get or create node

  msgAddr = uecho_message_getsourceaddress(msg);
  if (!msgAddr)
    return;

  node = uecho_controller_getnodebyaddress(ctrl, msgAddr);
  if (!node) {
    node = uecho_node_new();
    if (!node)
      return;
    uecho_node_setaddress(node, uecho_message_getsourceaddress(msg));
    uecho_controller_addnode(ctrl, node);
  }

  // Updated node

  propData = uecho_property_getdata(prop);

  for (idx = 1; (idx + 2) < propSize; idx += 3) {
    objCode = uecho_byte2integer((propData + idx), 3);
    uecho_node_setobject(node, objCode);
  }
}

/****************************************
 * uecho_controller_updatepropertydata
 ****************************************/

void uecho_controller_updatepropertydata(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoNode* srcNode;
  uEchoObject* srcObj;
  uEchoProperty* msgProp;
  uEchoPropertyCode msgPropCode;
  size_t msgOpc, n;

  srcNode = uecho_controller_getnodebyaddress(ctrl, uecho_message_getsourceaddress(msg));
  if (!srcNode)
    return;

  srcObj = uecho_node_getobjectbycode(srcNode, uecho_message_getsourceobjectcode(msg));
  if (!srcObj)
    return;

  msgOpc = uecho_message_getopc(msg);
  for (n = 0; n < msgOpc; n++) {
    msgProp = uecho_message_getproperty(msg, n);
    if (!msgProp)
      continue;
    msgPropCode = uecho_property_getcode(msgProp);
    if (!uecho_object_hasproperty(srcObj, msgPropCode)) {
      uecho_object_setproperty(srcObj, msgPropCode, uEchoPropertyAttrNone);
    }
    uecho_object_setpropertydata(srcObj, msgPropCode, uecho_property_getdata(msgProp), uecho_property_getdatasize(msgProp));
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
    if (ctrl->msgListener) {
      ctrl->msgListener(ctrl, msg);
    }
    return;
  }

  uecho_controller_handlerequestmessage(ctrl, msg);

  if (ctrl->msgListener) {
    ctrl->msgListener(ctrl, msg);
  }
}
