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

/****************************************
 * uecho_controller_handlesearchmessage
 ****************************************/

void uecho_controller_handlesearchmessage(uEchoController *ctrl, uEchoMessage *msg)
{
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
