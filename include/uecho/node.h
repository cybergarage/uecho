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
    
  struct uEchoMutex *mutex;
} uEchoNode, uEchoNodeList;

/****************************************
 * Function
 ****************************************/

uEchoNode *uecho_node_new();
void uecho_node_delete(uEchoNode *dev);
void uecho_node_clear(uEchoNode *dev);
#define uecho_node_next(dev) (uEchoNode *)uecho_list_next((uEchoList *)dev)
#define uecho_node_remove(dev) uecho_list_remove((uEchoList *)dev)

bool uecho_node_start(uEchoNode *dev);
bool uecho_node_stop(uEchoNode *dev);
bool uecho_node_isrunning(uEchoNode *dev);
	
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
