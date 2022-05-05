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

#define uEchoControllerPostResponseLoopCount 50

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
  ctrl->node = uecho_node_new();
  ctrl->nodes = uecho_nodelist_new();
  ctrl->option = uEchoOptionNone;

  server = uecho_node_getserver(ctrl->node);
  uecho_server_addobserver(server, ctrl, (uEchoMessageHandler)uecho_controller_servermessagelistener);

  uecho_controller_setuserdata(ctrl, NULL);
  uecho_controller_setlasttid(ctrl, 0);
  uecho_controller_setmessagelistener(ctrl, NULL);
  uecho_controller_setpostrequestmessage(ctrl, NULL);
  uecho_controller_setpostresponsemessage(ctrl, NULL);
  uecho_controller_setpostwaitemilitime(ctrl, uEchoControllerPostResponseMaxMiliTime);

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

  ctrl->msg_listener = listener;
}

/****************************************
 * uecho_controller_getmessagelistener
 ****************************************/

uEchoControllerMessageListener uecho_controller_getmessagelistener(uEchoController* ctrl)
{
  if (!ctrl)
    return NULL;

  return ctrl->msg_listener;
}

/****************************************
 * uecho_controller_hasmessageresponselistener
 ****************************************/

bool uecho_controller_hasmessagelistener(uEchoController* ctrl)
{
  if (!ctrl)
    return false;

  return ctrl->msg_listener ? true : false;
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoController* ctrl)
{
  bool all_actions_succeeded = true;

  if (!ctrl)
    return false;

  all_actions_succeeded &= uecho_nodelist_clear(ctrl->nodes);
  all_actions_succeeded &= uecho_node_start(ctrl->node);

  return all_actions_succeeded;
}

/****************************************
 * uecho_controller_stop
 ****************************************/

bool uecho_controller_stop(uEchoController* ctrl)
{
  bool all_actions_succeeded = true;

  if (!ctrl)
    return false;

  all_actions_succeeded &= uecho_node_stop(ctrl->node);

  return all_actions_succeeded;
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

uEchoObject* uecho_controller_getobjectbycodewithwait(uEchoController* ctrl, uEchoObjectCode code, clock_t wait_mili_time)
{
  uEchoObject* obj;
  int n;

  for (n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(wait_mili_time / uEchoWaitRetryCount);
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
  ctrl->user_data = data;
}

/****************************************
 * uecho_controller_getuserdata
 ****************************************/

void* uecho_controller_getuserdata(uEchoController* ctrl)
{
  if (!ctrl)
    return NULL;
  return ctrl->user_data;
}

/****************************************
 * uecho_controller_setlasttid
 ****************************************/

void uecho_controller_setlasttid(uEchoController* ctrl, uEchoTID tid)
{
  if (!ctrl)
    return;

  ctrl->last_tid = tid;
}

/****************************************
 * uecho_controller_gettid
 ****************************************/

uEchoTID uecho_controller_getlasttid(uEchoController* ctrl)
{
  if (!ctrl)
    return 0;

  return ctrl->last_tid;
}

/****************************************
 * uecho_controller_getnexttid
 ****************************************/

uEchoTID uecho_controller_getnexttid(uEchoController* ctrl)
{
  if (!ctrl)
    return 0;

  if (uEchoTidMax <= ctrl->last_tid) {
    ctrl->last_tid = 1;
  }
  else {
    ctrl->last_tid++;
  }
  return ctrl->last_tid;
}

/****************************************
 * uecho_controller_announcemessage
 ****************************************/

bool uecho_controller_announcemessage(uEchoController* ctrl, uEchoMessage* msg)
{
  uEchoObject* node_prof_obj;

  if (!ctrl || !msg)
    return false;

  node_prof_obj = uecho_node_getnodeprofileclassobject(ctrl->node);
  if (!node_prof_obj)
    return false;

  uecho_message_settid(msg, uecho_controller_getnexttid(ctrl));

  return uecho_object_announcemessage(node_prof_obj, msg);
}

/****************************************
 * uecho_controller_sendmessage
 ****************************************/

bool uecho_controller_sendmessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg)
{
  uEchoObject* node_prof_obj;

  if (!ctrl || !node || !msg || !ctrl->node)
    return false;

  uecho_message_settid(msg, uecho_controller_getnexttid(ctrl));

  node_prof_obj = uecho_node_getnodeprofileclassobject(ctrl->node);
  if (!node_prof_obj)
    return false;
  uecho_message_setsourceobjectcode(msg, uecho_object_getcode(node_prof_obj));

  return uecho_node_sendmessagebytes(ctrl->node, uecho_node_getaddress(node), uecho_message_getbytes(msg), uecho_message_size(msg));
}

/****************************************
 * uecho_controller_setpostrequestmessage
 ****************************************/

void uecho_controller_setpostrequestmessage(uEchoController* ctrl, uEchoMessage* msg)
{
  ctrl->post_req_msg = msg;
}

/****************************************
 * uecho_controller_getpostresponsemessage
 ****************************************/

uEchoMessage* uecho_controller_getpostrequestmessage(uEchoController* ctrl)
{
  return ctrl->post_req_msg;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_haspostrequestmessage(uEchoController* ctrl)
{
  return ctrl->post_req_msg ? true : false;
}

/****************************************
 * uecho_controller_setpostresponsemessage
 ****************************************/

void uecho_controller_setpostresponsemessage(uEchoController* ctrl, uEchoMessage* msg)
{
  ctrl->post_res_msg = msg;
}

/****************************************
 * uecho_controller_getpostresponsemessage
 ****************************************/

uEchoMessage* uecho_controller_getpostresponsemessage(uEchoController* ctrl)
{
  return ctrl->post_res_msg;
}

/****************************************
 * uecho_controller_haspostresponsemessage
 ****************************************/

bool uecho_controller_haspostresponsemessage(uEchoController* ctrl)
{
  return ctrl->post_res_msg ? true : false;
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

  return uecho_message_isresponsemessage(ctrl->post_req_msg, msg);
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

  return uecho_message_isresponsemessage(ctrl->post_req_msg, ctrl->post_res_msg);
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

  ctrl->post_res_wait_mili_time = mtime;
}

/****************************************
 * uecho_controller_getpostwaitemilitime
 ****************************************/

clock_t uecho_controller_getpostwaitemilitime(uEchoController* ctrl)
{
  if (!ctrl)
    return 0;

  return ctrl->post_res_wait_mili_time;
}

/****************************************
 * uecho_controller_postmessage
 ****************************************/

bool uecho_controller_postmessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* req_msg, uEchoMessage* res_msg)
{
  bool is_responce_received;
  int n;

  if (!ctrl)
    return false;

  uecho_mutex_lock(ctrl->mutex);

  uecho_controller_setpostrequestmessage(ctrl, req_msg);
  uecho_controller_setpostresponsemessage(ctrl, res_msg);

  if (!uecho_controller_sendmessage(ctrl, node, req_msg)) {
    uecho_mutex_unlock(ctrl->mutex);
    return false;
  }

  is_responce_received = false;
  for (n = 0; n < uEchoControllerPostResponseLoopCount; n++) {
    uecho_sleep(ctrl->post_res_wait_mili_time / uEchoControllerPostResponseLoopCount);
    if (uecho_controller_ispostresponsereceived(ctrl)) {
      is_responce_received = true;
      break;
    }
  }

  uecho_controller_setpostrequestmessage(ctrl, NULL);
  uecho_controller_setpostresponsemessage(ctrl, NULL);

  uecho_mutex_unlock(ctrl->mutex);

  return is_responce_received;
}

/****************************************
 * uecho_controller_searchwithesv
 ****************************************/

bool uecho_controller_searchwithesv(uEchoController* ctrl, uEchoEsv esv)
{
  uEchoMessage* msg;
  bool is_success;

  msg = uecho_message_search_new();
  if (!msg)
    return false;

  uecho_message_setesv(msg, esv);
  is_success = uecho_controller_announcemessage(ctrl, msg);
  uecho_message_delete(msg);

  return is_success;
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

bool uecho_controller_searchobjectwithesv(uEchoController* ctrl, byte obj_code, uEchoEsv esv)
{
  uEchoMessage* msg;
  bool is_success;

  if (!ctrl)
    return false;

  msg = uecho_message_search_new();

  if (!msg)
    return false;

  uecho_message_setesv(msg, esv);
  uecho_message_setdestinationobjectcode(msg, obj_code);
  is_success = uecho_controller_announcemessage(ctrl, msg);
  uecho_message_delete(msg);

  return is_success;
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoController* ctrl, byte obj_code)
{
  return uecho_controller_searchobjectwithesv(ctrl, obj_code, uEchoEsvReadRequest);
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
