/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_NODE_INTERNAL_H_
#define _UECHO_NODE_INTERNAL_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/class_internal.h>
#include <uecho/object_internal.h>
#include <uecho/core/server.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoNode {
  UECHO_LIST_STRUCT_MEMBERS
  
  uEchoMutex *mutex;
  uEchoServer *server;

  uEchoClassList *classes;
  uEchoObjectList *objects;
  
  void (*msgListener)(struct _uEchoNode *, uEchoMessage *); /* uEchoNodeMessageListener */
  char *address;
  uEchoOption option;
} uEchoNode, uEchoNodeList;

/****************************************
 * Header
 ****************************************/
  
#include <uecho/node.h>

/****************************************
 * Function
 ****************************************/

#define uecho_node_remove(node) uecho_list_remove((uEchoList *)node)
    
uEchoServer *uecho_node_getserver(uEchoNode *node);

void uecho_node_setoption(uEchoNode *node, uEchoOption value);
#define uecho_node_isoptionenabled(node, value) (node->option & value)
  
void uecho_node_servermessagelistener(uEchoServer *server, uEchoMessage *msg);

bool uecho_node_announceproperty(uEchoNode *node, uEchoProperty *prop);
bool uecho_node_announce(uEchoNode *node);
  
/****************************************
 * Function (Property)
 ****************************************/

uEchoNode *uecho_property_getnode(uEchoProperty *prop);

/****************************************
 * Function (Node List)
 ****************************************/
  
uEchoNodeList *uecho_nodelist_new(void);
void uecho_nodelist_delete(uEchoNodeList *nodes);
  
uEchoNode *uecho_nodelist_getbyaddress(uEchoNodeList *nodes, const char *addr);
  
#define uecho_nodelist_clear(nodes) uecho_list_clear((uEchoList *)nodes, (UECHO_LIST_DESTRUCTORFUNC)uecho_node_delete)
#define uecho_nodelist_size(nodes) uecho_list_size((uEchoList *)nodes)
#define uecho_nodelist_gets(nodes) (uEchoNode *)uecho_list_next((uEchoList *)nodes)
#define uecho_nodelist_add(nodes,node) uecho_list_add((uEchoList *)nodes, (uEchoList *)node)
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_INTERNAL_H_ */
