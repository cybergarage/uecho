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

#include <stdbool.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic UPnP device structure
 *
 * \note This struct can also be cast to a uEchoList* and used as a node in a 
 * linked list with uecho_list_* functions.
 */
typedef struct _uEchoNode
{
  /** Used by uecho_list_* functions to indicate start of list */
  bool headFlag;
  /** Used by uecho_list_* functions to point to the previous item in list */
  struct _uEchoNode *prev;
  /** Used by uecho_list_* functions to point to the next item in list */
  struct _uEchoNode *next;
    
  /** Mutex used in multithreading */
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
