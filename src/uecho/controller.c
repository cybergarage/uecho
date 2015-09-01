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
#include <uecho/profile.h>

#define uEchoControllerPostResponseLoopCount 50

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
  ctrl->option = uEchoOptionNone;
  
  server = uecho_node_getserver(ctrl->node);
  uecho_server_setuserdata(server, ctrl);
  uecho_server_setmessagelistener(server, uecho_controller_servermessagelistener);
  
  uecho_controller_setlasttid(ctrl, 0);
  uecho_controller_setmessagelistener(ctrl, NULL);
  uecho_controller_setpostrequestmessage(ctrl, NULL);
  uecho_controller_setpostresponsemessage(ctrl, NULL);
  uecho_controller_setpostwaitemilitime(ctrl, uEchoControllerPostResponseMaxMiliTime);
  
	return ctrl;
}

/****************************************
 * uecho_object_delete
 ****************************************/

void uecho_controller_delete(uEchoController *ctrl)
{
  if (!ctrl)
    return;
  
	uecho_controller_stop(ctrl);
	
	uecho_mutex_delete(ctrl->mutex);
  uecho_node_delete(ctrl->node);
  uecho_nodelist_delete(ctrl->nodes);

  free(ctrl);
}

/****************************************
 * uecho_controller_setmessagelistener
 ****************************************/

void uecho_controller_setmessagelistener(uEchoController *ctrl, uEchoControllerMessageListener listener)
{
  if (!ctrl)
    return;

  ctrl->msgListener = listener;
}

/****************************************
 * uecho_controller_getmessagelistener
 ****************************************/

uEchoControllerMessageListener uecho_controller_getmessagelistener(uEchoController *ctrl)
{
  if (!ctrl)
    return NULL;
  
  return ctrl->msgListener;
}

/****************************************
 * uecho_controller_hasmessageresponselistener
 ****************************************/

bool uecho_controller_hasmessagelistener(uEchoController *ctrl)
{
  if (!ctrl)
    return false;
  
  return ctrl->msgListener ? true : false;
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoController *ctrl)
{
  bool allActionsSucceeded = true;
  
  if (!ctrl)
    return false;
  
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
  
  if (!ctrl)
    return false;

  allActionsSucceeded &= uecho_node_stop(ctrl->node);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_isrunning
 ****************************************/

bool uecho_controller_isrunning(uEchoController *ctrl)
{
  if (!ctrl)
    return false;

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
  
  if (!ctrl)
    return NULL;

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
  
  if (!ctrl)
    return  NULL;
  
  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    obj = uecho_node_getobjectbycode(node, code);
    if (obj)
      return obj;
  }
  
  return NULL;
}

/****************************************
 * uecho_controller_enableoption
 ****************************************/

void uecho_controller_enableoption(uEchoController *ctrl, uEchoOption param)
{
  if (!ctrl)
    return;
  
  ctrl->option |= param;
  uecho_node_setoption(ctrl->node, ctrl->option);
}

/****************************************
 * uecho_controller_disableoption
 ****************************************/

void uecho_controller_disableoption(uEchoController *ctrl, uEchoOption param)
{
  if (!ctrl)
    return;
  
  ctrl->option ^= param;
  uecho_node_setoption(ctrl->node, ctrl->option);
}

/****************************************
 * uecho_controller_isoptionenabled
 ****************************************/

bool uecho_controller_isoptionenabled(uEchoController *ctrl, uEchoOption param)
{
  if (!ctrl)
    return false;
  
  return (ctrl->option & param) ? true : false;
}

/****************************************
 * uecho_controller_setlasttid
 ****************************************/

void uecho_controller_setlasttid(uEchoController *ctrl, uEchoTID tid)
{
  if (!ctrl)
    return;
  
  ctrl->lastTID = tid;
}

/****************************************
 * uecho_controller_gettid
 ****************************************/

uEchoTID uecho_controller_getlasttid(uEchoController *ctrl)
{
  if (!ctrl)
    return 0;

  return ctrl->lastTID;
}

/****************************************
 * uecho_controller_getnexttid
 ****************************************/

uEchoTID uecho_controller_getnexttid(uEchoController *ctrl)
{
  if (!ctrl)
    return 0;
  
  if (uEchoTidMax <= ctrl->lastTID) {
    ctrl->lastTID = 1;
  }
  else {
    ctrl->lastTID++;
  }
  return ctrl->lastTID;
}

/****************************************
 * uecho_controller_announcemessage
 ****************************************/

bool uecho_controller_announcemessage(uEchoController *ctrl, uEchoMessage *msg)
{
  uEchoObject *nodeProfObj;
  
  if (!ctrl || !msg)
    return false;
  
  nodeProfObj = uecho_node_getnodeprofileclassobject(ctrl->node);
  if (!nodeProfObj)
    return false;
  
  uecho_message_settid(msg, uecho_controller_getnexttid(ctrl));
  
  return uecho_object_announcemessage(nodeProfObj, msg);
}

/****************************************
 * uecho_controller_sendmessage
 ****************************************/

bool uecho_controller_sendmessage(uEchoController *ctrl, uEchoObject *obj, uEchoMessage *msg)
{
  uEchoObject *nodeProfObj;
  
  if (!ctrl || !obj || !msg)
    return false;

  nodeProfObj = uecho_node_getnodeprofileclassobject(ctrl->node);
  if (!nodeProfObj)
    return false;

  uecho_message_settid(msg, uecho_controller_getnexttid(ctrl));
  
  return uecho_object_sendmessage(nodeProfObj, obj, msg);
}

/****************************************
 * uecho_controller_setpostrequestmessage
 ****************************************/

void uecho_controller_setpostrequestmessage(uEchoController *ctrl, uEchoMessage *msg)
{
  ctrl->postReqMsg = msg;
}

/****************************************
 * uecho_controller_getpostresponsemessage
 ****************************************/

uEchoMessage *uecho_controller_getpostrequestmessage(uEchoController *ctrl)
{
  return ctrl->postReqMsg;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_haspostrequestmessage(uEchoController *ctrl)
{
  return ctrl->postReqMsg ? true : false;
}

/****************************************
 * uecho_controller_setpostresponsemessage
 ****************************************/

void uecho_controller_setpostresponsemessage(uEchoController *ctrl, uEchoMessage *msg)
{
  ctrl->postResMsg = msg;
}

/****************************************
 * uecho_controller_getpostresponsemessage
 ****************************************/

uEchoMessage *uecho_controller_getpostresponsemessage(uEchoController *ctrl)
{
  return ctrl->postResMsg;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_haspostresponsemessage(uEchoController *ctrl)
{
  return ctrl->postResMsg ? true : false;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_ispostresponsemessage(uEchoController *ctrl, uEchoMessage *msg)
{
  if (!ctrl)
    return false;
  
  if (!uecho_controller_haspostrequestmessage(ctrl))
    return false;
  
  return uecho_message_isresponsemessage(ctrl->postReqMsg, msg);
}

/****************************************
 * uecho_controller_ispostresponsereceived
 ****************************************/

bool uecho_controller_ispostresponsereceived(uEchoController *ctrl)
{
  if (!ctrl)
    return false;
  
  if (!uecho_controller_haspostrequestmessage(ctrl) || !uecho_controller_haspostresponsemessage(ctrl))
    return false;
  
  return uecho_message_isresponsemessage(ctrl->postReqMsg, ctrl->postResMsg);
}

/****************************************
 * uecho_controller_ispostresponsewaiting
 ****************************************/

bool uecho_controller_ispostresponsewaiting(uEchoController *ctrl)
{
  if (!uecho_controller_haspostrequestmessage(ctrl) || !uecho_controller_haspostresponsemessage(ctrl))
    return false;
  
  return true;
}

/****************************************
 * uecho_controller_setpostwaitemilitime
 ****************************************/

void uecho_controller_setpostwaitemilitime(uEchoController *ctrl, clock_t mtime)
{
  if (!ctrl)
    return;
  
  ctrl->postResWaitMiliTime = mtime;
}

/****************************************
 * uecho_controller_getpostwaitemilitime
 ****************************************/

clock_t uecho_controller_getpostwaitemilitime(uEchoController *ctrl)
{
  if (!ctrl)
    return 0;
  
  return ctrl->postResWaitMiliTime;
}

/****************************************
 * uecho_controller_postmessage
 ****************************************/

bool uecho_controller_postmessage(uEchoController *ctrl, uEchoObject *obj, uEchoMessage *reqMsg, uEchoMessage *resMsg)
{
  bool isResponceReceived;
  int n;
  
  if (!ctrl)
    return false;
  
  uecho_mutex_lock(ctrl->mutex);
  
  uecho_controller_setpostrequestmessage(ctrl, reqMsg);
  uecho_controller_setpostresponsemessage(ctrl, resMsg);
  
  if (!uecho_controller_sendmessage(ctrl, obj, reqMsg))
    return false;

  isResponceReceived = false;
  for (n=0; n<uEchoControllerPostResponseLoopCount; n++) {
    uecho_sleep(ctrl->postResWaitMiliTime / uEchoControllerPostResponseLoopCount);
    if (uecho_controller_ispostresponsereceived(ctrl)) {
      isResponceReceived = true;
      break;
    }
  }
  
  uecho_controller_setpostrequestmessage(ctrl, NULL);
  uecho_controller_setpostresponsemessage(ctrl, NULL);
  
  uecho_mutex_unlock(ctrl->mutex);
  
  return isResponceReceived;
}

/****************************************
 * uecho_controller_searchallobjects
 ****************************************/

bool uecho_controller_searchallobjects(uEchoController *ctrl)
{
  uEchoMessage *msg;
  
  msg = uecho_message_search_new();

  return uecho_controller_announcemessage(ctrl, msg);
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoController *ctrl, byte objCode)
{
  uEchoMessage *msg;
  uEchoProperty *prop;
  
  if (!ctrl)
    return false;

  msg = uecho_message_search_new();
  
  uecho_message_setdestinationobjectcode(msg, objCode);
  
  prop = uecho_message_getproperty(msg, 0);
  uecho_property_setcode(prop, uEchoNodeProfileClassOperatingStatus);
  uecho_property_setdata(prop, NULL, 0);
  
  return uecho_controller_announcemessage(ctrl, msg);
}
