/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROPERTY_H_
#define _UECHO_PROPERTY_H_

#include <stdbool.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoProperty
{
  bool headFlag;
  struct _uEchoProperty *prev;
  struct _uEchoProperty *next;
    
} uEchoProperty, uEchoPropertyList;

/****************************************
 * Function
 ****************************************/

uEchoProperty *uecho_property_new();
void uecho_property_delete(uEchoProperty *dev);
void uecho_property_clear(uEchoProperty *dev);
#define uecho_property_next(dev) (uEchoProperty *)uecho_list_next((uEchoList *)dev)
#define uecho_property_remove(dev) uecho_list_remove((uEchoList *)dev)

bool uecho_property_start(uEchoProperty *dev);
bool uecho_property_stop(uEchoProperty *dev);
bool uecho_property_isrunning(uEchoProperty *dev);
	
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
