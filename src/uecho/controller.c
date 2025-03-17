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
#include <uecho/profile.h>

#define uEchoControllerPostResponseLoopCount (CLOCKS_PER_SEC / 10)

/****************************************
 * uecho_controller_new
 ****************************************/

uEchoController* uecho_controller_new(void)
{
  uEchoController* ctrl;
  uEchoServer* server;

  ctrl = (uEchoController*)malloc(sizeof(uEchoController));

  if (!ctrl)
    return NULL;

  ctrl->mutex = uecho_mutex_new();
  ctrl->cond = uecho_cond_new();
  ctrl->node = uecho_node_new();
  ctrl->nodes = uecho_nodelist_new();
  ctrl->option = uEchoOptionNone;

  server = uecho_node_getserver(ctrl->node);
  uecho_server_addobserver(server, ctrl, (uEchoMessageHandler)uecho_controller_servermessagelistener);

  uecho_controller_setuserdata(ctrl, NULL);
  uecho_controller_setmessagelistener(ctrl, NULL);
  uecho_controller_setmessagelistener(ctrl, NULL);
  uecho_controller_setnodelistener(ctrl, NULL);
  uecho_controller_setpostresponsemessage(ctrl, NULL);
  uecho_controller_setpostwaitemilitime(ctrl, UECHO_CONTROLLER_POST_RESPONSE_MAX_CLOCK_TIME);

  return ctrl;
}

/****************************************
 * uecho_controller_delete
 ****************************************/

bool uecho_controller_delete(uEchoController* ctrl)
{
  if (!ctrl)
    return false;

  uecho_controller_stop(ctrl);

  uecho_mutex_delete(ctrl->mutex);
  uecho_cond_delete(ctrl->cond);
  uecho_node_delete(ctrl->node);
  uecho_nodelist_delete(ctrl->nodes);

  free(ctrl);

  return true;
}

/****************************************
 * uecho_controller_setmessagelistener
 ****************************************/

void uecho_controller_setmessagelistener(uEchoController* ctrl, uEchoControllerMessageListener listener)
{
  if (!ctrl)
    return;

  ctrl->msgListener = listener;
}

/****************************************
 * uecho_controller_setnodelistener
 ****************************************/

void uecho_controller_setnodelistener(uEchoController* ctrl, uEchoControllerNodeListener listener)
{
  if (!ctrl)
    return;

  ctrl->nodeListener = listener;
}

/****************************************
 * uecho_controller_getmessagelistener
 ****************************************/

uEchoControllerMessageListener uecho_controller_getmessagelistener(uEchoController* ctrl)
{
  if (!ctrl)
    return NULL;

  return ctrl->msgListener;
}

/****************************************
 * uecho_controller_hasmessageresponselistener
 ****************************************/

bool uecho_controller_hasmessagelistener(uEchoController* ctrl)
{
  if (!ctrl)
    return false;

  return ctrl->msgListener ? true : false;
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoController* ctrl)
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

bool uecho_controller_stop(uEchoController* ctrl)
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

bool uecho_controller_isrunning(uEchoController* ctrl)
{
  if (!ctrl)
    return false;

  if (!uecho_node_isrunning(ctrl->node))
    return false;

  return true;
}

/****************************************
 * uecho_controller_getlocalnode
 ****************************************/

uEchoNode* uecho_controller_getlocalnode(uEchoController* ctrl)
{
  if (!ctrl)
    return NULL;

  return ctrl->node;
}

/****************************************
 * uecho_controller_addnode
 ****************************************/

bool uecho_controller_addnode(uEchoController* ctrl, uEchoNode* node)
{
  if (!uecho_nodelist_add(ctrl->nodes, node))
    return false;
  uecho_node_setcontroller(node, ctrl);
  return true;
}

/****************************************
 * uecho_controller_getnodecount
 ****************************************/

size_t uecho_controller_getnodecount(uEchoController* ctrl)
{
  return uecho_nodelist_size(ctrl->nodes);
}

/****************************************
 * uecho_controller_getnodes
 ****************************************/

uEchoNode* uecho_controller_getnodes(uEchoController* ctrl)
{
  return uecho_nodelist_gets(ctrl->nodes);
}

/****************************************
 * uecho_controller_getnodebyaddress
 ****************************************/

uEchoNode* uecho_controller_getnodebyaddress(uEchoController* ctrl, const char* addr)
{
  uEchoNode* node;

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

uEchoObject* uecho_controller_getobjectbycode(uEchoController* ctrl, uEchoObjectCode code)
{
  uEchoNode* node;
  uEchoObject* obj;

  if (!ctrl)
    return NULL;

  for (node = uecho_controller_getnodes(ctrl); node; node = uecho_node_next(node)) {
    obj = uecho_node_getobjectbycode(node, code);
    if (obj)
      return obj;
  }

  return NULL;
}

/****************************************
 * uecho_controller_getobjectbycodewithwait
 ****************************************/

uEchoObject* uecho_controller_getobjectbycodewithwait(uEchoController* ctrl, uEchoObjectCode code, clock_t waitMiliTime)
{
  uEchoObject* obj;
  int n;

  for (n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(waitMiliTime / uEchoWaitRetryCount);
    obj = uecho_controller_getobjectbycode(ctrl, code);
    if (obj)
      return obj;
  }

  return NULL;
}

/****************************************
 * uecho_controller_enableoption
 ****************************************/

void uecho_controller_enableoption(uEchoController* ctrl, uEchoOption param)
{
  if (!ctrl)
    return;

  ctrl->option |= param;
  uecho_node_setoption(ctrl->node, ctrl->option);
}

/****************************************
 * uecho_controller_disableoption
 ****************************************/

void uecho_controller_disableoption(uEchoController* ctrl, uEchoOption param)
{
  if (!ctrl)
    return;

  ctrl->option ^= param;
  uecho_node_setoption(ctrl->node, ctrl->option);
}

/****************************************
 * uecho_controller_isoptionenabled
 ****************************************/

bool uecho_controller_isoptionenabled(uEchoController* ctrl, uEchoOption param)
{
  if (!ctrl)
    return false;

  return (ctrl->option & param) ? true : false;
}

/****************************************
 * uecho_controller_disableudpserver
 ****************************************/

void uecho_controller_disableudpserver(uEchoController* ctrl)
{
  uecho_controller_enableoption(ctrl, uEchoControllerOptionDisableUdpServer);
}

/****************************************
 * uecho_controller_setuserdata
 ****************************************/

void uecho_controller_setuserdata(uEchoController* ctrl, void* data)
{
  if (!ctrl)
    return;
  ctrl->userData = data;
}

/****************************************
 * uecho_controller_getuserdata
 ****************************************/

void* uecho_controller_getuserdata(uEchoController* ctrl)
{
  if (!ctrl)
    return NULL;
  return ctrl->userData;
}

/****************************************
 * uecho_controller_announcemessage
 ****************************************/

bool uecho_controller_announcemessage(uEchoController* ctrl, uEchoMessage* msg)
{
  if (!ctrl || !msg)
    return false;
  return uecho_node_announcemessage(ctrl->node, msg);
}

/****************************************
 * uecho_controller_sendmessage
 ****************************************/

bool uecho_controller_sendmessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg)
{
  if (!ctrl || !node || !msg || !ctrl->node)
    return false;
  return uecho_node_sendmessage(ctrl->node, node, msg);
}

/****************************************
 * uecho_controller_setpostrequestmessage
 ****************************************/

void uecho_controller_setpostrequestmessage(uEchoController* ctrl, uEchoMessage* msg)
{
  ctrl->postReqMsg = msg;
}

/****************************************
 * uecho_controller_getpostresponsemessage
 ****************************************/

uEchoMessage* uecho_controller_getpostrequestmessage(uEchoController* ctrl)
{
  return ctrl->postReqMsg;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_haspostrequestmessage(uEchoController* ctrl)
{
  return ctrl->postReqMsg ? true : false;
}

/****************************************
 * uecho_controller_setpostresponsemessage
 ****************************************/

void uecho_controller_setpostresponsemessage(uEchoController* ctrl, uEchoMessage* msg)
{
  ctrl->postResMsg = msg;
}

/****************************************
 * uecho_controller_getpostresponsemessage
 ****************************************/

uEchoMessage* uecho_controller_getpostresponsemessage(uEchoController* ctrl)
{
  return ctrl->postResMsg;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_haspostresponsemessage(uEchoController* ctrl)
{
  return ctrl->postResMsg ? true : false;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_ispostresponsemessage(uEchoController* ctrl, uEchoMessage* msg)
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

bool uecho_controller_ispostresponsereceived(uEchoController* ctrl)
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

bool uecho_controller_ispostresponsewaiting(uEchoController* ctrl)
{
  if (!uecho_controller_haspostrequestmessage(ctrl) || !uecho_controller_haspostresponsemessage(ctrl))
    return false;

  return true;
}

/****************************************
 * uecho_controller_setpostwaitemilitime
 ****************************************/

void uecho_controller_setpostwaitemilitime(uEchoController* ctrl, clock_t mtime)
{
  if (!ctrl)
    return;

  ctrl->postResWaitClockTime = mtime;
}

/****************************************
 * uecho_controller_getpostwaitemilitime
 ****************************************/

clock_t uecho_controller_getpostwaitemilitime(uEchoController* ctrl)
{
  if (!ctrl)
    return 0;

  return ctrl->postResWaitClockTime;
}

/****************************************
 * uecho_controller_postmessage
 ****************************************/

bool uecho_controller_postmessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* reqMsg, uEchoMessage* resMsg)
{
  bool isResponceReceived;
  int n;

  if (!ctrl)
    return false;

  uecho_mutex_lock(ctrl->mutex);

  uecho_controller_setpostrequestmessage(ctrl, reqMsg);
  uecho_controller_setpostresponsemessage(ctrl, resMsg);

  if (!uecho_controller_sendmessage(ctrl, node, reqMsg)) {
    uecho_mutex_unlock(ctrl->mutex);
    return false;
  }

#if defined(USE_SLEEP_WAIT)
  is_responce_received = false;
  for (n = 0; n < uEchoControllerPostResponseLoopCount; n++) {
    uecho_sleep(ctrl->post_res_wait_clock_time / uEchoControllerPostResponseLoopCount);
    if (uecho_controller_ispostresponsereceived(ctrl)) {
      is_responce_received = true;
      break;
    }
  }
#else
  isResponceReceived = uecho_cond_timedwait(ctrl->cond, ctrl->postResWaitClockTime);
#endif

  uecho_controller_setpostrequestmessage(ctrl, NULL);
  uecho_controller_setpostresponsemessage(ctrl, NULL);

  uecho_mutex_unlock(ctrl->mutex);

  return isResponceReceived;
}

/****************************************
 * uecho_controller_searchwithesv
 ****************************************/

bool uecho_controller_searchwithesv(uEchoController* ctrl, uEchoEsv esv)
{
  uEchoMessage* msg;
  bool isSuccess;

  msg = uecho_message_search_new();
  if (!msg)
    return false;

  uecho_message_setesv(msg, esv);
  isSuccess = uecho_controller_announcemessage(ctrl, msg);
  uecho_message_delete(msg);

  return isSuccess;
}

/****************************************
 * uecho_controller_search
 ****************************************/

bool uecho_controller_search(uEchoController* ctrl)
{
  return uecho_controller_searchwithesv(ctrl, uEchoEsvReadRequest);
}

/****************************************
 * uecho_controller_searchobjectwithesv
 ****************************************/

bool uecho_controller_searchobjectwithesv(uEchoController* ctrl, byte objCode, uEchoEsv esv)
{
  uEchoMessage* msg;
  bool isSuccess;

  if (!ctrl)
    return false;

  msg = uecho_message_search_new();

  if (!msg)
    return false;

  uecho_message_setesv(msg, esv);
  uecho_message_setdestinationobjectcode(msg, objCode);
  isSuccess = uecho_controller_announcemessage(ctrl, msg);
  uecho_message_delete(msg);

  return isSuccess;
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoController* ctrl, byte objCode)
{
  return uecho_controller_searchobjectwithesv(ctrl, objCode, uEchoEsvReadRequest);
}

/****************************************
 * uecho_node_setcontroller
 ****************************************/

void uecho_node_setcontroller(uEchoNode* node, uEchoController* ctrl)
{
  if (!node)
    return;
  node->controller = ctrl;
}

/****************************************
 * uecho_node_getcontroller
 ****************************************/

uEchoController* uecho_node_getcontroller(uEchoNode* node)
{
  if (!node)
    return NULL;
  return node->controller;
}
