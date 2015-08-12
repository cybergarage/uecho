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

uEchoNode *uecho_node_new();
void uecho_node_delete(uEchoNode *node);
void uecho_node_clear(uEchoNode *node);
#define uecho_node_next(node) (uEchoNode *)uecho_list_next((uEchoList *)node)
#define uecho_node_remove(node) uecho_list_remove((uEchoList *)node)

bool uecho_node_start(uEchoNode *node);
bool uecho_node_stop(uEchoNode *node);
bool uecho_node_isrunning(uEchoNode *node);
	
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
