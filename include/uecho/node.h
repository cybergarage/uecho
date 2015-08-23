/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_NODE_H_
#define _UECHO_NODE_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/class.h>
#include <uecho/object.h>
#include <uecho/core/server.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoNode
{
  bool headFlag;
  struct _uEchoNode *prev;
  struct _uEchoNode *next;
    
  uEchoMutex *mutex;
  uEchoServer *server;

  uEchoClassList *classes;
  uEchoObjectList *objects;
  
} uEchoNode, uEchoNodeList;

/****************************************
 * Function
 ****************************************/

uEchoNode *uecho_node_new(void);
void uecho_node_delete(uEchoNode *node);
void uecho_node_clear(uEchoNode *node);
#define uecho_node_next(node) (uEchoNode *)uecho_list_next((uEchoList *)node)
#define uecho_node_remove(node) uecho_list_remove((uEchoList *)node)

uEchoClass *uecho_node_getclasses(uEchoNode *node);
uEchoClass *uecho_node_getclassbycode(uEchoNode *node, uEchoClassCode code);
bool uecho_node_hasclassbycode(uEchoNode *node, uEchoClassCode code);
size_t uecho_node_getclasscount(uEchoNode *node);

uEchoObject *uecho_node_getobjects(uEchoNode *node);
uEchoObject *uecho_node_getobjectbycode(uEchoNode *node, uEchoObjectCode code);
bool uecho_node_hasobjectbycode(uEchoNode *node, uEchoObjectCode code);
size_t uecho_node_getobjectcount(uEchoNode *node);

bool uecho_node_addobject(uEchoNode *node, uEchoObject *obj);
bool uecho_node_setobject(uEchoNode *node, uEchoObjectCode code);
  
bool uecho_node_start(uEchoNode *node);
bool uecho_node_stop(uEchoNode *node);
bool uecho_node_isrunning(uEchoNode *node);

bool uecho_node_postsearch(uEchoNode *node, byte *msg, size_t msgLen);
  
/****************************************
 * Function (Object)
 ****************************************/
  
void uecho_object_setparentnode(uEchoObject *obj, uEchoNode *node);
uEchoNode *uecho_object_getparentnode(uEchoObject *obj);

/****************************************
 * Macro
 ****************************************/
  
#define uecho_node_getnodeprofileclassobject(node) uecho_node_getobjectbycode(node, uEchoNodeProfileObject)
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
