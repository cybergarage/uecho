/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_NODE_H_
#define _UECHO_NODE_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/object.h>

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

bool uecho_node_start(uEchoNode *node);
bool uecho_node_stop(uEchoNode *node);
bool uecho_node_isrunning(uEchoNode *node);
	
/****************************************
 * Macro
 ****************************************/
  
#if defined(C99)
  
inline bool uecho_node_setobject(uEchoNode *node, uEchoObjectCode code) {return uecho_objectlist_set(node->objects, code);}
inline uEchoObject *uecho_node_getobjects(uEchoNode *node) {return uecho_objectlist_gets(node->objects);}
inline uEchoObject *uecho_node_getobjectbycode(uEchoNode *node, uEchoObjectCode code) {return uecho_objectlist_getbycode(node->objects, code);}
inline size_t uecho_node_getobjectcount(uEchoNode *node) {return uecho_objectlist_size(node->objects);}
  
#else
  
#define uecho_node_setobject(node, code) uecho_objectlist_set(node->objects, code)
#define uecho_node_getobjects(node) uecho_objectlist_gets(node->objects)
#define uecho_node_getobjectbycode(node, code) uecho_objectlist_getbycode(node->objects, code)
#define uecho_node_getobjectcount(node) uecho_objectlist_size(node->objects)
  
#endif

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
