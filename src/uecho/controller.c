/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller.h>
#include <uecho/profile.h>

void uecho_controller_servermessagelistener(uEchoServer *server, uEchoMessage *msg);

/****************************************
 * uecho_controller_new
 ****************************************/

uEchoController *uecho_controller_new(void)
{
	uEchoController *ctrl;
  uEchoServer *server;

  ctrl = (uEchoController *)malloc(sizeof(uEchoController));

  if (!ctrl)
    return NULL;

  ctrl->mutex = uecho_mutex_new();
  ctrl->node = uecho_node_new();
  ctrl->nodes = uecho_nodelist_new();
  
  server = uecho_node_getserver(ctrl->node);
  uecho_server_setuserdata(server, ctrl);
  uecho_server_setmessagelistener(server, uecho_controller_servermessagelistener);
  
  uecho_controller_setlasttid(ctrl, 0);
  uecho_controller_setmessageresponselistener(ctrl, NULL);
  
	return ctrl;
}

/****************************************
 * uecho_object_delete
 ****************************************/

void uecho_controller_delete(uEchoController *ctrl)
{
	uecho_controller_stop(ctrl);
	
	uecho_mutex_delete(ctrl->mutex);
  uecho_node_delete(ctrl->node);
  uecho_nodelist_delete(ctrl->nodes);

  free(ctrl);
}

/****************************************
 * uecho_controller_setmessageresponselistener
 ****************************************/

void uecho_controller_setmessageresponselistener(uEchoController *ctrl, uEchoControllerMessageListener listener)
{
  ctrl->msgListener = listener;
}

/****************************************
 * uecho_controller_getmessageresponselistener
 ****************************************/

uEchoControllerMessageListener uecho_controller_getmessageresponselistener(uEchoController *ctrl)
{
  return ctrl->msgListener;
}

/****************************************
 * uecho_controller_hasmessageresponselistener
 ****************************************/

bool uecho_controller_hasmessageresponselistener(uEchoController *ctrl)
{
  return ctrl->msgListener ? true : false;
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoController *ctrl)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_nodelist_clear(ctrl->nodes);
  allActionsSucceeded &= uecho_node_start(ctrl->node);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_stop
 ****************************************/

bool uecho_controller_stop(uEchoController *ctrl)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_node_stop(ctrl->node);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_isrunning
 ****************************************/

bool uecho_controller_isrunning(uEchoController *ctrl)
{
  if (!uecho_node_isrunning(ctrl->node))
    return false;
	return true;
}

/****************************************
 * uecho_controller_addnode
 ****************************************/

bool uecho_controller_addnode(uEchoController *ctrl, uEchoNode *node)
{
  return uecho_nodelist_add(ctrl->nodes, node);
}

/****************************************
 * uecho_controller_getnodecount
 ****************************************/

size_t uecho_controller_getnodecount(uEchoController *ctrl)
{
  return uecho_nodelist_size(ctrl->nodes);
}

/****************************************
 * uecho_controller_getnodes
 ****************************************/

uEchoNode *uecho_controller_getnodes(uEchoController *ctrl)
{
  return uecho_nodelist_gets(ctrl->nodes);
}

/****************************************
 * uecho_controller_getnodebyaddress
 ****************************************/

uEchoNode *uecho_controller_getnodebyaddress(uEchoController *ctrl, const char *addr)
{
  uEchoNode *node;
  
  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    if (uecho_node_isaddress(node, addr))
      return node;
  }
  
  return NULL;
}

/****************************************
 * uecho_controller_getobjectbycode
 ****************************************/

uEchoObject *uecho_controller_getobjectbycode(uEchoController *ctrl, uEchoObjectCode code)
{
  uEchoNode *node;
  uEchoObject *obj;
  
  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    obj = uecho_node_getobjectbycode(node, code);
    if (obj)
      return obj;
  }
  
  return NULL;
}

/****************************************
 * uecho_controller_setlasttid
 ****************************************/

void uecho_controller_setlasttid(uEchoController *ctrl, uEchoTID tid)
{
  ctrl->lastTID = tid;
}

/****************************************
 * uecho_controller_gettid
 ****************************************/

uEchoTID uecho_controller_getlasttid(uEchoController *ctrl)
{
  return ctrl->lastTID;
}

/****************************************
 * uecho_controller_getnexttid
 ****************************************/

uEchoTID uecho_controller_getnexttid(uEchoController *ctrl)
{
  if (uEchoTidMax <= ctrl->lastTID) {
    ctrl->lastTID = 1;
  }
  else {
    ctrl->lastTID++;
  }
  return ctrl->lastTID;
}

/****************************************
 * uecho_controller_searchallobjects
 ****************************************/

bool uecho_controller_postsearch(uEchoController *ctrl, uEchoMessage *msg)
{
  byte *msgBytes;
  size_t msgLen;
  
  uecho_message_settid(msg, uecho_controller_getnexttid(ctrl));
  uecho_message_setsourceobjectcode(msg, uEchoNodeProfileObject);
  
  msgBytes = uecho_message_getbytes(msg);
  msgLen = uecho_message_size(msg);
  
  return uecho_node_announcemessagebytes(ctrl->node, msgBytes, msgLen);
}

/****************************************
 * uecho_controller_searchallobjects
 ****************************************/

bool uecho_controller_searchallobjects(uEchoController *ctrl)
{
  uEchoMessage *msg;
  
  msg = uecho_message_search_new();

  return uecho_controller_postsearch(ctrl, msg);
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoController *ctrl, byte objCode)
{
  uEchoMessage *msg;
  uEchoProperty *prop;
  
  msg = uecho_message_search_new();
  
  uecho_message_setdestinationobjectcode(msg, objCode);
  
  prop = uecho_message_getproperty(msg, 0);
  uecho_property_setcode(prop, uEchoNodeProfileClassOperatingStatus);
  uecho_property_setdata(prop, NULL, 0);
  
  return uecho_controller_postsearch(ctrl, msg);
}
