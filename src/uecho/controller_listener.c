/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
  uEchoProperty *prop;
  uEchoObjectCode objCode;
  byte *propData;
  size_t propSize, instanceSize;
  size_t idx;
  
  prop = uecho_message_getpropertybycode(msg, uEchoNodeProfileClassSelfNodeInstanceListS);
  if (!prop)
    return;
  
  propSize = uecho_property_getdatasize(prop);
  if (propSize < 1)
    return;

  propData = uecho_property_getdata(prop);

  instanceSize = propData[0];
  
  for (idx=1; (idx+2)<propSize; idx+=3) {
    objCode = uecho_byte2integer((propData + idx), 3);
  }
}

/****************************************
 * uecho_controller_handlerequestmessage
 ****************************************/

void uecho_controller_handlerequestmessage(uEchoController *ctrl, uEchoMessage *msg)
{
  if (uecho_message_issearchrequest(msg)) {
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
