/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/node.h>
#include <uecho/controller.h>
#include <uecho/profile.h>
#include <uecho/misc.h>

/****************************************
 * uecho_controller_handlesearchmessage
 ****************************************/

void uecho_controller_handlesearchmessage(uEchoController *ctrl, uEchoMessage *msg)
{
  uEchoNode *node;
  uEchoProperty *prop;
  uEchoObjectCode objCode;
  byte *propData;
  size_t propSize, instanceSize;
  size_t idx;
  const char *msgAddr;

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
    uecho_controller_addnode(ctrl, node);
  }
  
  // Updated node
  
  propData = uecho_property_getdata(prop);

  instanceSize = propData[0];
  
  for (idx=1; (idx+2)<propSize; idx+=3) {
    objCode = uecho_byte2integer((propData + idx), 3);
    uecho_node_setobject(node, objCode);
  }
}

/****************************************
 * uecho_controller_handlerequestmessage
 ****************************************/

void uecho_controller_handlerequestmessage(uEchoController *ctrl, uEchoMessage *msg)
{
  if (uecho_message_issearchresponse(msg)) {
    uecho_controller_handlesearchmessage(ctrl, msg);
    return;
  }
}

/****************************************
* uecho_controller_servermessagelistener
****************************************/

void uecho_controller_servermessagelistener(uEchoServer *server, uEchoMessage *msg)
{
  uEchoController *ctrl;
  uEchoObjectCode destObjCode;
  
  ctrl = (uEchoController *)uecho_server_getuserdata(server);
  if (!ctrl)
    return;

  destObjCode = uecho_message_getdestinationobjectcode(msg);
  
  uecho_controller_handlerequestmessage(ctrl, msg);
}
