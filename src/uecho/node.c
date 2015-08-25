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
#include <uecho/profile.h>
#include <uecho/misc.h>

void uecho_node_servermessagelistener(uEchoServer *server, uEchoMessage *msg);

/****************************************
* uecho_node_new
****************************************/

uEchoNode *uecho_node_new(void)
{
	uEchoNode *node;
  uEchoObject *obj;

	node = (uEchoNode *)malloc(sizeof(uEchoNode));

  if (!node)
    return NULL;
        
  uecho_list_node_init((uEchoList *)node);
  
  node->mutex = uecho_mutex_new();
  node->classes = uecho_classlist_new();
  node->objects = uecho_objectlist_new();

  node->server = uecho_server_new();
  uecho_server_setuserdata(node->server, node);
  uecho_server_setmessagelistener(node->server, uecho_node_servermessagelistener);
  
  obj = uecho_nodeprofileclass_new();
  uecho_node_addobject(node, obj);
  
	return node;
}

/****************************************
* uecho_node_delete
****************************************/

void uecho_node_delete(uEchoNode *node)
{
	uecho_list_remove((uEchoList *)node);
  
  uecho_mutex_delete(node->mutex);
  uecho_classlist_delete(node->classes);
  uecho_objectlist_delete(node->objects);
  uecho_server_delete(node->server);

	free(node);
}

/****************************************
 * uecho_node_clear
 ****************************************/

void uecho_node_clear(uEchoNode *node)
{
  uecho_classlist_clear(node->classes);
  uecho_objectlist_clear(node->objects);
}

/****************************************
 * uecho_node_getserver
 ****************************************/

uEchoServer *uecho_node_getserver(uEchoNode *node)
{
  return node->server;
}

/****************************************
 * uecho_node_getclasses
 ****************************************/

uEchoClass *uecho_node_getclasses(uEchoNode *node)
{
  return uecho_classlist_gets(node->classes);
}

/****************************************
 * uecho_node_getclassbycode
 ****************************************/

uEchoClass *uecho_node_getclassbycode(uEchoNode *node, uEchoObjectCode code)
{
  return uecho_classlist_getbycode(node->classes, code);
}

/****************************************
 * uecho_node_hasclassbycode
 ****************************************/

bool uecho_node_hasclassbycode(uEchoNode *node, uEchoClassCode code)
{
  return uecho_node_getclassbycode(node, code) ? true : false;
}

/****************************************
 * uecho_node_getclasscount
 ****************************************/

size_t uecho_node_getclasscount(uEchoNode *node)
{
  return uecho_classlist_size(node->classes);
}

/****************************************
 * uecho_node_getobjects
 ****************************************/

uEchoObject *uecho_node_getobjects(uEchoNode *node)
{
  return uecho_objectlist_gets(node->objects);
}
    
/****************************************
 * uecho_node_getobjectbycode
 ****************************************/

uEchoObject *uecho_node_getobjectbycode(uEchoNode *node, uEchoObjectCode code)
{
  return uecho_objectlist_getbycode(node->objects, code);
}

/****************************************
 * uecho_node_hasobjectbycode
 ****************************************/

bool uecho_node_hasobjectbycode(uEchoNode *node, uEchoObjectCode code)
{
  return uecho_node_getobjectbycode(node, code) ? true : false;
}

/****************************************
 * uecho_node_getobjectcount
 ****************************************/

size_t uecho_node_getobjectcount(uEchoNode *node)
{
   return uecho_objectlist_size(node->objects);
}

/****************************************
 * uecho_node_addobject
 ****************************************/

bool uecho_node_updatenodeprofileclass(uEchoNode *node)
{
  uEchoObject *obj;
  
  obj = uecho_node_getobjectbycode(node, uEchoNodeProfileObject);
  if (!obj)
    return false;
  
  return uecho_nodeprofileclass_updateinstanceproperties(obj);
}

/****************************************
 * uecho_node_setobject
 ****************************************/

bool uecho_node_setobject(uEchoNode *node, uEchoObjectCode code)
{
  uEchoObject *obj;
  
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

bool uecho_node_addobject(uEchoNode *node, uEchoObject *obj)
{
  uEchoObjectCode objCode;
  uEchoClassCode clsCode;
  
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

bool uecho_node_start(uEchoNode *node)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_server_start(node->server);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_node_stop
 ****************************************/

bool uecho_node_stop(uEchoNode *node)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_server_stop(node->server);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_node_isrunning
 ****************************************/

bool uecho_node_isrunning(uEchoNode *node)
{
  if (!uecho_server_isrunning(node->server))
    return false;
  return true;
}

/****************************************
 * uecho_node_postsearch
 ****************************************/

bool uecho_node_postsearch(uEchoNode *node, byte *msgBytes, size_t msgLen)
{
  return uecho_server_postsearch(node->server, msgBytes, msgLen);
}


