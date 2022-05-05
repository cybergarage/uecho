/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_node.h>

#include <uecho/misc.h>
#include <uecho/profile.h>

/****************************************
* uecho_node_new
****************************************/

uEchoNode* uecho_node_new(void)
{
  uEchoNode* node;
  uEchoObject* obj;

  node = (uEchoNode*)malloc(sizeof(uEchoNode));

  if (!node)
    return NULL;

  uecho_list_node_init((uEchoList*)node);

  node->mutex = uecho_mutex_new();
  
  node->controller = NULL;
  node->classes = uecho_classlist_new();
  node->objects = uecho_objectlist_new();

  node->server = uecho_server_new();
  uecho_server_addobserver(node->server, node, (uEchoMessageHandler)uecho_node_servermessagelistener);

  node->address = uecho_string_new();
  uecho_node_setmessagelistener(node, NULL);

  obj = uecho_nodeprofile_new();
  uecho_node_addobject(node, obj);

  return node;
}

/****************************************
* uecho_node_delete
****************************************/

bool uecho_node_delete(uEchoNode* node)
{
  if (!node)
    return false;

  uecho_list_remove((uEchoList*)node);

  uecho_mutex_delete(node->mutex);
  uecho_classlist_delete(node->classes);
  uecho_objectlist_delete(node->objects);
  uecho_server_delete(node->server);
  uecho_string_delete(node->address);

  free(node);

  return true;
}

/****************************************
 * uecho_node_clear
 ****************************************/

bool uecho_node_clear(uEchoNode* node)
{
  if (!node)
    return false;

  uecho_classlist_clear(node->classes);
  uecho_objectlist_clear(node->objects);

  return true;
}

/****************************************
 * uecho_node_next
 ****************************************/

uEchoNode* uecho_node_next(uEchoNode* node)
{
  return (uEchoNode*)uecho_list_next((uEchoList*)node);
}

/****************************************
 * uecho_node_setoption
 ****************************************/

void uecho_node_setoption(uEchoNode* node, uEchoOption value)
{
  if (!node)
    return;

  node->option = value;
  uecho_server_setoption(node->server, value);
}

/****************************************
 * uecho_node_setmessagelistener
 ****************************************/

void uecho_node_setmessagelistener(uEchoNode* node, uEchoNodeMessageListener listener)
{
  if (!node)
    return;

  node->msg_listener = listener;
}

/****************************************
 * uecho_node_getmessagelistener
 ****************************************/

uEchoNodeMessageListener uecho_node_getmessagelistener(uEchoNode* node)
{
  if (!node)
    return NULL;

  return node->msg_listener;
}

/****************************************
 * uecho_node_hasmessagelistener
 ****************************************/

bool uecho_node_hasmessagelistener(uEchoNode* node)
{
  if (!node)
    return false;

  return node->msg_listener ? true : false;
}

/****************************************
 * uecho_node_setaddress
 ****************************************/

void uecho_node_setaddress(uEchoNode* node, const char* addr)
{
  if (!node)
    return;
  uecho_string_setvalue(node->address, addr);
}

/****************************************
 * uecho_node_getaddress
 ****************************************/

const char* uecho_node_getaddress(uEchoNode* node)
{
  if (!node)
    return NULL;
  return uecho_string_getvalue(node->address);
}

/****************************************
 * uecho_node_isaddress
 ****************************************/

bool uecho_node_isaddress(uEchoNode* node, const char* addr)
{
  if (!node)
    return false;

  if (node->address) {
    if (uecho_streq(uecho_string_getvalue(node->address), addr))
      return true;
  }

  if (uecho_server_isboundaddress(node->server, addr))
    return true;

  return false;
}

/****************************************
 * uecho_node_setmanufacturercode
 ****************************************/

bool uecho_node_setmanufacturercode(uEchoNode* node, uEchoManufactureCode code)
{
  uEchoObject* obj;

  if (!node)
    return false;

  for (obj = uecho_node_getobjects(node); obj; obj = uecho_object_next(obj)) {
    uecho_object_setmanufacturercode(obj, code);
  }

  return true;
}

/****************************************
 * uecho_node_getserver
 ****************************************/

uEchoServer* uecho_node_getserver(uEchoNode* node)
{
  if (!node)
    return NULL;

  return node->server;
}

/****************************************
 * uecho_node_getclasses
 ****************************************/

uEchoClass* uecho_node_getclasses(uEchoNode* node)
{
  if (!node)
    return 0;

  return uecho_classlist_gets(node->classes);
}

/****************************************
 * uecho_node_getclassbycode
 ****************************************/

uEchoClass* uecho_node_getclassbycode(uEchoNode* node, uEchoObjectCode code)
{
  return uecho_classlist_getbycode(node->classes, code);
}

/****************************************
 * uecho_node_hasclassbycode
 ****************************************/

bool uecho_node_hasclassbycode(uEchoNode* node, uEchoClassCode code)
{
  return uecho_node_getclassbycode(node, code) ? true : false;
}

/****************************************
 * uecho_node_getclasscount
 ****************************************/

size_t uecho_node_getclasscount(uEchoNode* node)
{
  return uecho_classlist_size(node->classes);
}

/****************************************
 * uecho_node_getobjects
 ****************************************/

uEchoObject* uecho_node_getobjects(uEchoNode* node)
{
  return uecho_objectlist_gets(node->objects);
}

/****************************************
 * uecho_node_getobjectbycode
 ****************************************/

uEchoObject* uecho_node_getobjectbycode(uEchoNode* node, uEchoObjectCode code)
{
  return uecho_objectlist_getbycode(node->objects, code);
}

/****************************************
 * uecho_node_hasobjectbycode
 ****************************************/

bool uecho_node_hasobjectbycode(uEchoNode* node, uEchoObjectCode code)
{
  return uecho_node_getobjectbycode(node, code) ? true : false;
}

/****************************************
 * uecho_node_getobjectcount
 ****************************************/

size_t uecho_node_getobjectcount(uEchoNode* node)
{
  return uecho_objectlist_size(node->objects);
}

/****************************************
 * uecho_node_addobject
 ****************************************/

bool uecho_node_updatenodeprofileclass(uEchoNode* node)
{
  uEchoObject* obj;

  if (!node)
    return false;

  obj = uecho_node_getobjectbycode(node, uEchoNodeProfileObject);
  if (!obj)
    return false;

  return uecho_nodeprofile_updateinstanceproperties(obj);
}

/****************************************
 * uecho_node_setobject
 ****************************************/

bool uecho_node_setobject(uEchoNode* node, uEchoObjectCode code)
{
  uEchoObject* obj;

  if (!node)
    return false;

  obj = uecho_objectlist_getbycode(node->objects, code);
  if (obj)
    return true;

  obj = uecho_object_new();
  if (!obj)
    return false;

  uecho_object_setcode(obj, code);

  return uecho_node_addobject(node, obj);
}

/****************************************
 * uecho_node_addobject
 ****************************************/

bool uecho_node_addobject(uEchoNode* node, uEchoObject* obj)
{
  uEchoObjectCode obj_code;
  uEchoClassCode cls_code;

  if (!node || !obj)
    return false;

  obj_code = uecho_object_getcode(obj);
  if (uecho_node_getobjectbycode(node, obj_code))
    return false;

  if (!uecho_objectlist_add(node->objects, obj))
    return false;

  uecho_object_setparentnode(obj, node);

  cls_code = uecho_objectcode2classcode(obj_code);
  uecho_classlist_set(node->classes, cls_code);

  if (!uecho_node_updatenodeprofileclass(node))
    return false;

  return true;
}

/****************************************
 * uecho_node_start
 ****************************************/

bool uecho_node_start(uEchoNode* node)
{
  bool all_actions_succeeded = true;

  all_actions_succeeded &= uecho_server_start(node->server);

  // 4.3.1 Basic Sequence for ECHONET Lite Node Startup
  all_actions_succeeded &= uecho_node_announce(node);

  return all_actions_succeeded;
}

/****************************************
 * uecho_node_stop
 ****************************************/

bool uecho_node_stop(uEchoNode* node)
{
  bool all_actions_succeeded = true;

  if (!node)
    return false;

  all_actions_succeeded &= uecho_server_stop(node->server);

  return all_actions_succeeded;
}

/****************************************
 * uecho_node_isrunning
 ****************************************/

bool uecho_node_isrunning(uEchoNode* node)
{
  if (!node)
    return false;

  if (!uecho_server_isrunning(node->server))
    return false;

  return true;
}

/****************************************
 * uecho_node_announcemessagebytes
 ****************************************/

bool uecho_node_announcemessagebytes(uEchoNode* node, byte* msg_bytes, size_t msg_len)
{
  if (!node)
    return false;
  return uecho_server_postannounce(node->server, msg_bytes, msg_len);
}

/****************************************
 * uecho_node_sendmessagebytes
 ****************************************/

bool uecho_node_sendmessagebytes(uEchoNode* node, const char* addr, byte* msg, size_t msg_len)
{
  if (!node || !addr || !msg || (msg_len <= 0))
    return false;
  return uecho_server_postresponse(node->server, addr, msg, msg_len);
}

/****************************************
 * uecho_node_announcemessage
 ****************************************/

bool uecho_node_announcemessage(uEchoNode* node, uEchoMessage* msg)
{
  if (!node || !msg)
    return false;
  return uecho_node_announcemessagebytes(node, uecho_message_getbytes(msg), uecho_message_size(msg));
}

/****************************************
 * uecho_node_sendmessage
 ****************************************/

bool uecho_node_sendmessage(uEchoNode* node, uEchoNode* dst_node, uEchoMessage* msg)
{
  if (!node || !dst_node || !msg)
    return false;
  return uecho_node_sendmessagebytes(node, uecho_node_getaddress(dst_node), uecho_message_getbytes(msg), uecho_message_size(msg));
}

/****************************************
 * uecho_node_announceproperty
 ****************************************/

bool uecho_node_announceproperty(uEchoNode* node, uEchoObject* obj, uEchoProperty* prop)
{
  uEchoMessage* msg;
  bool is_success;

  if (!node || !obj || !prop)
    return false;
  
  msg = uecho_message_new();
  if (!msg)
    return false;

  uecho_message_setesv(msg, uEchoEsvNotification);
  uecho_message_setsourceobjectcode(msg, uecho_object_getcode(obj));
  uecho_message_setdestinationobjectcode(msg, uEchoNodeProfileObject);
  uecho_message_addproperty(msg, uecho_property_copy(prop));

  is_success = uecho_node_announcemessage(node, msg);

  uecho_message_delete(msg);

  return is_success;
}

/****************************************
 * uecho_node_announce
 ****************************************/

bool uecho_node_announce(uEchoNode* node)
{
  uEchoObject* node_obj;
  uEchoProperty* node_prop;

  node_obj = uecho_node_getnodeprofileclassobject(node);
  if (!node_obj)
    return false;

  node_prop = uecho_object_getproperty(node_obj, uEchoNodeProfileClassInstanceListNotification);
  if (!node_prop)
    return false;

  return uecho_node_announceproperty(node, node_obj, node_prop);
}
