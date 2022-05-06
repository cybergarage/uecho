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

#include <uecho/_class.h>
#include <uecho/_object.h>

#include <uecho/frame/server.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/util/strings.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoNode {
  UECHO_LIST_STRUCT_MEMBERS

  uEchoMutex* mutex;
  uEchoServer* server;
  uEchoTID last_tid;

  void* controller;

  uEchoClassList* classes;
  uEchoObjectList* objects;

  void (*msg_listener)(struct UEchoNode*, uEchoMessage*); /* uEchoNodeMessageListener */
  uEchoString* address;
  uEchoOption option;
} uEchoNode, uEchoNodeList;

/****************************************
 * Header
 ****************************************/

#include <uecho/node.h>

/****************************************
 * Function
 ****************************************/

#define uecho_node_remove(node) uecho_list_remove((uEchoList*)node)

uEchoServer* uecho_node_getserver(uEchoNode* node);

void uecho_node_setoption(uEchoNode* node, uEchoOption value);
#define uecho_node_isoptionenabled(node, value) (node->option & value)

void uecho_node_servermessagelistener(uEchoNode* node, uEchoMessage* msg);

bool uecho_node_announceproperty(uEchoNode* node, uEchoObject* obj, uEchoProperty* prop);
bool uecho_node_announce(uEchoNode* node);

uEchoNodeMessageListener uecho_node_getmessagelistener(uEchoNode* node);
bool uecho_node_hasmessagelistener(uEchoNode* node);

void uecho_node_setlasttid(uEchoNode* node, uEchoTID tid);
uEchoTID uecho_node_getlasttid(uEchoNode* node);
uEchoTID uecho_node_getnexttid(uEchoNode* node);

/****************************************
 * Function (Object)
 ****************************************/

bool uecho_object_setparentnode(uEchoObject* obj, uEchoNode* node);

/****************************************
 * Function (Property)
 ****************************************/

uEchoNode* uecho_property_getnode(uEchoProperty* prop);

/****************************************
 * Function (Node List)
 ****************************************/

uEchoNodeList* uecho_nodelist_new(void);
void uecho_nodelist_delete(uEchoNodeList* nodes);

uEchoNode* uecho_nodelist_getbyaddress(uEchoNodeList* nodes, const char* addr);

#define uecho_nodelist_clear(nodes) uecho_list_clear((uEchoList*)nodes, (UECHO_LIST_DESTRUCTORFUNC)uecho_node_delete)
#define uecho_nodelist_size(nodes) uecho_list_size((uEchoList*)nodes)
#define uecho_nodelist_gets(nodes) (uEchoNode*)uecho_list_next((uEchoList*)nodes)
#define uecho_nodelist_add(nodes, node) uecho_list_add((uEchoList*)nodes, (uEchoList*)node)

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_INTERNAL_H_ */
