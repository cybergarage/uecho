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
	uEchoController *cp;
  uEchoServer *server;

  cp = (uEchoController *)malloc(sizeof(uEchoController));

  if (!cp)
    return NULL;

  cp->mutex = uecho_mutex_new();
  cp->node = uecho_node_new();
  cp->nodes = uecho_nodelist_new();
  
  server = uecho_node_getserver(cp->node);
  uecho_server_setuserdata(server, cp);
  uecho_server_setmessagelistener(server, uecho_controller_servermessagelistener);
  
  uecho_controller_setlasttid(cp, 0);
  
	return cp;
}

/****************************************
 * uecho_object_delete
 ****************************************/

void uecho_controller_delete(uEchoController *cp)
{
	uecho_controller_stop(cp);
	
	uecho_mutex_delete(cp->mutex);
  uecho_node_delete(cp->node);
  uecho_nodelist_delete(cp->nodes);

  free(cp);
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoController *cp)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_nodelist_clear(cp->nodes);
  allActionsSucceeded &= uecho_node_start(cp->node);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_stop
 ****************************************/

bool uecho_controller_stop(uEchoController *cp)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_node_stop(cp->node);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_isrunning
 ****************************************/

bool uecho_controller_isrunning(uEchoController *cp)
{
  if (!uecho_node_isrunning(cp->node))
    return false;
	return true;
}

/****************************************
 * uecho_controller_addnode
 ****************************************/

bool uecho_controller_addnode(uEchoController *cp, uEchoNode *node)
{
  return uecho_nodelist_add(cp->nodes, node);
}

/****************************************
 * uecho_controller_getnodes
 ****************************************/

uEchoNode *uecho_controller_getnodes(uEchoController *cp)
{
  return uecho_nodelist_gets(cp->nodes);
}

/****************************************
 * uecho_controller_getnodebyaddress
 ****************************************/

uEchoNode *uecho_controller_getnodebyaddress(uEchoController *cp, const char *addr)
{
  uEchoNode *node;
  
  for (node = uecho_controller_getnodes(cp); node; node = uecho_node_next(node)) {
    if (uecho_node_isaddress(node, addr))
      return node;
  }
  
  return NULL;
}

/****************************************
 * uecho_controller_getobjectbycode
 ****************************************/

uEchoObject *uecho_controller_getobjectbycode(uEchoController *cp, uEchoObjectCode code)
{
  uEchoNode *node;
  uEchoObject *obj;
  
  for (node = uecho_controller_getnodes(cp); node; node = uecho_node_next(node)) {
    obj = uecho_node_getobjectbycode(node, code);
    if (obj)
      return obj;
  }
  
  return NULL;
}

/****************************************
 * uecho_controller_setlasttid
 ****************************************/

void uecho_controller_setlasttid(uEchoController *cp, uEchoTID tid)
{
  cp->lastTID = tid;
}

/****************************************
 * uecho_controller_gettid
 ****************************************/

uEchoTID uecho_controller_getlasttid(uEchoController *cp)
{
  return cp->lastTID;
}

/****************************************
 * uecho_controller_getnexttid
 ****************************************/

uEchoTID uecho_controller_getnexttid(uEchoController *cp)
{
  if (uEchoTidMax <= cp->lastTID) {
    cp->lastTID = 1;
  }
  else {
    cp->lastTID++;
  }
  return cp->lastTID;
}

/****************************************
 * uecho_controller_searchallobjects
 ****************************************/

bool uecho_controller_postsearch(uEchoController *cp, uEchoMessage *msg)
{
  byte *msgBytes;
  size_t msgLen;
  
  uecho_message_settid(msg, uecho_controller_getnexttid(cp));
  uecho_message_setsourceobjectcode(msg, uEchoNodeProfileObject);
  
  msgBytes = uecho_message_getbytes(msg);
  msgLen = uecho_message_size(msg);
  
  return uecho_node_postannounce(cp->node, msgBytes, msgLen);
}

/****************************************
 * uecho_controller_searchallobjects
 ****************************************/

bool uecho_controller_searchallobjects(uEchoController *cp)
{
  uEchoMessage *msg;
  
  msg = uecho_message_search_new();

  return uecho_controller_postsearch(cp, msg);
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoController *cp, byte objCode)
{
  uEchoMessage *msg;
  uEchoProperty *prop;
  
  msg = uecho_message_search_new();
  
  uecho_message_setdestinationobjectcode(msg, objCode);
  
  prop = uecho_message_getproperty(msg, 0);
  uecho_property_setcode(prop, uEchoNodeProfileClassOperatingStatus);
  uecho_property_setdata(prop, NULL, 0);
  
  return uecho_controller_postsearch(cp, msg);
}
