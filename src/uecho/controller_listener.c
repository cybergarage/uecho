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
  uEchoObjectCode objCode;
  byte* propData;
  size_t propSize;
  size_t idx;
  const char* msgAddr;
  bool nodeAdded;
  bool nodeUpdated;

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

  nodeAdded = false;
  node = uecho_controller_getnodebyaddress(ctrl, msgAddr);
  if (!node) {
    node = uecho_node_new();
    if (!node)
      return;
    uecho_node_setaddress(node, uecho_message_getsourceaddress(msg));
    uecho_controller_addnode(ctrl, node);
    nodeAdded = true;
  }

  // Updated node

  nodeUpdated = false;
  propData = uecho_property_getdata(prop);
  for (idx = 1; (idx + 2) < propSize; idx += 3) {
    objCode = uecho_byte2integer((propData + idx), 3);
    if (uecho_node_hasobjectbycode(node, objCode))
      continue;
    uecho_node_setobject(node, objCode);
    nodeUpdated = true;
  }

  // Notify node status

  if (ctrl->nodeListener) {
    if (nodeAdded) {
      ctrl->nodeListener(ctrl, node, uEchoNodeStatusAdded, msg);
    }
    else if (nodeUpdated) {
      ctrl->nodeListener(ctrl, node, uEchoNodeStatusUpdated, msg);
    }
  }
}

/****************************************
 * uecho_controller_updatepropertydata
 ****************************************/

bool uecho_controller_updateopcpropertydata(uEchoController* ctrl, uEchoObject* obj, byte opc, uEchoProperty** ep)
{
  uEchoProperty* msgProp;
  uEchoPropertyCode msgPropCode;
  byte* msgData;
  size_t msgDataSize;
  uEchoProperty* objProp;
  bool objPropUpdated;
  size_t n;

  objPropUpdated = false;
  for (n = 0; n < opc; n++) {
    msgProp = ep[n];
    if (!msgProp)
      continue;

    msgPropCode = uecho_property_getcode(msgProp);
    if (!uecho_object_hasproperty(obj, msgPropCode)) {
      uecho_object_setproperty(obj, msgPropCode, uEchoPropertyAttrNone);
    }

    objProp = uecho_object_getproperty(obj, msgPropCode);
    if (!objProp)
      continue;

    msgData = uecho_property_getdata(msgProp);
    msgDataSize = uecho_property_getdatasize(msgProp);
    if (uecho_property_isdataequal(objProp, msgData, msgDataSize))
      continue;

    uecho_property_setdata(objProp, msgData, msgDataSize);

    objPropUpdated = true;
  }

  return objPropUpdated;
}

bool uecho_controller_updatenodebyresponsemessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg)
{
  uEchoObject* nodeObj;
  bool nodeUpdated;

  nodeObj = uecho_node_getobjectbycode(node, uecho_message_getsourceobjectcode(msg));
  if (!nodeObj)
    return false;

  nodeUpdated = false;
  if (uecho_message_isreadwritemessage(msg)) {
    nodeUpdated = uecho_controller_updateopcpropertydata(ctrl, nodeObj, msg->opcGet, msg->epGet);
  }
  else {
    nodeUpdated = uecho_controller_updateopcpropertydata(ctrl, nodeObj, msg->opc, msg->ep);
  }

  return nodeUpdated;
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
  bool nodeUpdated;

  nodeUpdated = false;
  if (uecho_message_isnotification(msg) || uecho_message_isreadresponse(msg) || uecho_message_isnotifyresponse(msg)) {
    nodeUpdated = uecho_controller_updatenodebyresponsemessage(ctrl, node, msg);
  }

  // Notify node status

  if (ctrl->nodeListener) {
    if (uecho_message_isnotification(msg)) {
      ctrl->nodeListener(ctrl, node, uEchoNodeStatusAnnounced, msg);
    }
    if (uecho_message_isresponse(msg)) {
      ctrl->nodeListener(ctrl, node, uEchoNodeStatusResponded, msg);
    }
    if (nodeUpdated) {
      ctrl->nodeListener(ctrl, node, uEchoNodeStatusUpdated, msg);
    }
  }
}

/****************************************
 * uecho_controller_servermessagelistener
 ****************************************/

void uecho_controller_servermessagelistener(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoNode* srcNode;

  if (!ctrl || !msg)
    return;

  if (ctrl->msgListener) {
    ctrl->msgListener(ctrl, msg);
  }

  if (uecho_controller_ispostresponsewaiting(ctrl)) {
    uecho_controller_handlepostresponse(ctrl, msg);
  }

  if (uecho_message_issearchresponse(msg)) {
    uecho_controller_handlesearchmessage(ctrl, msg);
    return;
  }

  if (uecho_node_hasobjectbycode(ctrl->node, uecho_message_getdestinationobjectcode(msg))) {
    srcNode = uecho_controller_getnodebyaddress(ctrl, uecho_message_getsourceaddress(msg));
    if (srcNode) {
      uecho_controller_handlenodemessage(ctrl, srcNode, msg);
    }
  }
}
