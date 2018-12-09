/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/misc.h>
#include <uecho/node_internal.h>
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
  node->classes = uecho_classlist_new();
  node->objects = uecho_objectlist_new();

  node->server = uecho_server_new();
  uecho_server_setuserdata(node->server, node);
  uecho_server_setmessagelistener(node->server, uecho_node_servermessagelistener);

  node->address = NULL;
  uecho_node_setmessagelistener(node, NULL);

  obj = uecho_nodeprofileclass_new();
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

  node->msgListener = listener;
}

/****************************************
 * uecho_node_getmessagelistener
 ****************************************/

uEchoNodeMessageListener uecho_node_getmessagelistener(uEchoNode* node)
{
  if (!node)
    return NULL;

  return node->msgListener;
}

/****************************************
 * uecho_node_hasmessagelistener
 ****************************************/

bool uecho_node_hasmessagelistener(uEchoNode* node)
{
  if (!node)
    return false;

  return node->msgListener ? true : false;
}

/****************************************
 * uecho_node_setaddress
 ****************************************/

void uecho_node_setaddress(uEchoNode* node, const char* addr)
{
  if (!node)
    return;
  uecho_strloc(addr, &node->address);
}

/****************************************
 * uecho_node_getaddress
 ****************************************/

const char* uecho_node_getaddress(uEchoNode* node)
{
  if (!node)
    return NULL;

  return node->address;
}

/****************************************
 * uecho_node_isaddress
 ****************************************/

bool uecho_node_isaddress(uEchoNode* node, const char* addr)
{
  if (!node)
    return false;

  if (node->address) {
    if (uecho_streq(node->address, addr))
      return true;
  }

  if (uecho_server_isboundaddress(node->server, addr))
    return true;

  return false;
}

/****************************************
 * uecho_node_setmanufacturercode
 ****************************************/

bool uecho_node_setmanufacturercode(uEchoNode* node, uEchoManufacturerCode code)
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

  return uecho_nodeprofileclass_updateinstanceproperties(obj);
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
  uEchoObjectCode objCode;
  uEchoClassCode clsCode;

  if (!node || !obj)
    return false;

  objCode = uecho_object_getcode(obj);
  if (uecho_node_getobjectbycode(node, objCode))
    return false;

  if (!uecho_objectlist_add(node->objects, obj))
    return false;

  uecho_object_setparentnode(obj, node);

  clsCode = uecho_objectcode2classcode(objCode);
  uecho_classlist_set(node->classes, clsCode);

  if (!uecho_node_updatenodeprofileclass(node))
    return false;

  return true;
}

/****************************************
 * uecho_node_start
 ****************************************/

bool uecho_node_start(uEchoNode* node)
{
  bool allActionsSucceeded = true;

  allActionsSucceeded &= uecho_server_start(node->server);

  // 4.3.1 Basic Sequence for ECHONET Lite Node Startup
  allActionsSucceeded &= uecho_node_announce(node);

  return allActionsSucceeded;
}

/****************************************
 * uecho_node_stop
 ****************************************/

bool uecho_node_stop(uEchoNode* node)
{
  bool allActionsSucceeded = true;

  if (!node)
    return false;

  allActionsSucceeded &= uecho_server_stop(node->server);

  return allActionsSucceeded;
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

bool uecho_node_announcemessagebytes(uEchoNode* node, byte* msgBytes, size_t msgLen)
{
  return uecho_server_postannounce(node->server, msgBytes, msgLen);
}

/****************************************
 * uecho_node_sendmessagebytes
 ****************************************/

bool uecho_node_sendmessagebytes(uEchoNode* node, const char* addr, byte* msg, size_t msgLen)
{
  return uecho_server_postresponse(node->server, addr, msg, msgLen);
}

/****************************************
 * uecho_node_announcemessage
 ****************************************/

bool uecho_node_announcemessage(uEchoNode* node, uEchoMessage* msg)
{
  return uecho_node_announcemessagebytes(node, uecho_message_getbytes(msg), uecho_message_size(msg));
}

/****************************************
 * uecho_node_sendmessage
 ****************************************/

bool uecho_node_sendmessage(uEchoNode* node, uEchoNode* dstNode, uEchoMessage* msg)
{
  return uecho_node_sendmessagebytes(node, uecho_node_getaddress(dstNode), uecho_message_getbytes(msg), uecho_message_size(msg));
}

/****************************************
 * uecho_node_announceproperty
 ****************************************/

bool uecho_node_announceproperty(uEchoNode* node, uEchoProperty* prop)
{
  uEchoMessage* msg;
  bool isSuccess;

  msg = uecho_message_new();
  if (!msg)
    return false;

  uecho_message_setesv(msg, uEchoEsvNotification);
  uecho_message_setsourceobjectcode(msg, uEchoNodeProfileObject);
  uecho_message_setdestinationobjectcode(msg, uEchoNodeProfileObject);
  uecho_message_addproperty(msg, uecho_property_copy(prop));

  isSuccess = uecho_node_announcemessage(node, msg);

  uecho_message_delete(msg);

  return isSuccess;
}

/****************************************
 * uecho_node_announce
 ****************************************/

bool uecho_node_announce(uEchoNode* node)
{
  uEchoObject* nodeObj;
  uEchoProperty* nodeProp;

  nodeObj = uecho_node_getnodeprofileclassobject(node);
  if (!nodeObj)
    return false;

  nodeProp = uecho_object_getproperty(nodeObj, uEchoNodeProfileClassInstanceListNotification);
  if (!nodeProp)
    return false;

  return uecho_node_announceproperty(node, nodeProp);
}
