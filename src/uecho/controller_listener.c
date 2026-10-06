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
  propData = uecho_property_getdata(prop);
  if (propSize != 1 + (size_t)propData[0] * 3 || (uecho_message_getsourceobjectcode(msg) != uEchoNodeProfileObject && uecho_message_getsourceobjectcode(msg) != uEchoNodeProfileObjectReadOnly))
    return;

  // Get or create node

  msgAddr = uecho_message_getsourceaddress(msg);
  if (!msgAddr || !*msgAddr)
    return;

  uecho_mutex_lock(ctrl->nodesMutex);
  nodeAdded = false;
  node = uecho_controller_getnodebyaddress(ctrl, msgAddr);
  if (!node) {
    if (uecho_controller_getnodecount(ctrl) >= UECHO_CONTROLLER_MAX_NODES) {
      uecho_mutex_unlock(ctrl->nodesMutex);
      return;
    }
    node = uecho_node_new();
    if (!node) {
      uecho_mutex_unlock(ctrl->nodesMutex);
      return;
    }
    uecho_node_setaddress(node, uecho_message_getsourceaddress(msg));
    uecho_controller_addnode(ctrl, node);
    nodeAdded = true;
  }

  // Updated node

  nodeUpdated = false;
  uecho_mutex_lock(node->mutex);
  propData = uecho_property_getdata(prop);
  for (idx = 1; (idx + 2) < propSize; idx += 3) {
    objCode = uecho_byte2integer((propData + idx), 3);
    if (uecho_node_hasobjectbycode(node, objCode))
      continue;
    if (uecho_node_getobjectcount(node) >= UECHO_CONTROLLER_MAX_OBJECTS_PER_NODE)
      break;
    if (uecho_node_setobject(node, objCode))
      nodeUpdated = true;
  }
  uecho_mutex_unlock(node->mutex);
  uecho_mutex_unlock(ctrl->nodesMutex);

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
  uEchoNode* parent = uecho_object_getparentnode(obj);
  uEchoMutex* mutex = parent ? parent->mutex : NULL;
  uecho_mutex_lock(mutex);
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

  uecho_mutex_unlock(mutex);
  return objPropUpdated;
}

bool uecho_controller_updatenodebyresponsemessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg)
{
  uEchoObject* nodeObj;
  bool nodeUpdated;

  uecho_mutex_lock(node->mutex);
  nodeObj = uecho_node_getobjectbycode(node, uecho_message_getsourceobjectcode(msg));
  if (!nodeObj) {
    uecho_mutex_unlock(node->mutex);
    return false;
  }

  nodeUpdated = false;
  if (uecho_message_isreadwritemessage(msg)) {
    nodeUpdated = uecho_controller_updateopcpropertydata(ctrl, nodeObj, msg->opcGet, msg->epGet);
  }
  else {
    nodeUpdated = uecho_controller_updateopcpropertydata(ctrl, nodeObj, msg->opc, msg->ep);
  }

  uecho_mutex_unlock(node->mutex);
  return nodeUpdated;
}

/****************************************
 * uecho_controller_handlepostresponse
 ****************************************/

void uecho_controller_handlepostresponse(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoCond* const condition = ctrl->cond;
  pthread_mutex_lock(&condition->mutexId);
  if (uecho_controller_ispostresponsewaiting(ctrl) && !uecho_controller_ispostresponsereceived(ctrl)
      && uecho_controller_ispostresponsemessage(ctrl, msg)
      && uecho_message_set(uecho_controller_getpostresponsemessage(ctrl), msg))
    pthread_cond_signal(&condition->condId);
  pthread_mutex_unlock(&condition->mutexId);
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

  uecho_controller_handlepostresponse(ctrl, msg);

  if (uecho_message_issearchresponse(msg)) {
    uecho_controller_handlesearchmessage(ctrl, msg);
    return;
  }

  if (uecho_node_hasobjectbycode(ctrl->node, uecho_message_getdestinationobjectcode(msg))) {
    uecho_mutex_lock(ctrl->nodesMutex);
    srcNode = uecho_controller_getnodebyaddress(ctrl, uecho_message_getsourceaddress(msg));
    uecho_mutex_unlock(ctrl->nodesMutex);
    if (srcNode) {
      uecho_controller_handlenodemessage(ctrl, srcNode, msg);
    }
  }
}
