/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_OBJECT_H_
#define _UECHO_OBJECT_H_

#include <stdbool.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoObject
{
  bool headFlag;
  struct _uEchoObject *prev;
  struct _uEchoObject *next;
    
} uEchoObject, uEchoObjectList;

/****************************************
 * Function
 ****************************************/

uEchoObject *uecho_object_new();
void uecho_object_delete(uEchoObject *dev);
void uecho_object_clear(uEchoObject *dev);
#define uecho_object_next(dev) (uEchoObject *)uecho_list_next((uEchoList *)dev)
#define uecho_object_remove(dev) uecho_list_remove((uEchoList *)dev)

bool uecho_object_start(uEchoObject *dev);
bool uecho_object_stop(uEchoObject *dev);
bool uecho_object_isrunning(uEchoObject *dev);
	
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
