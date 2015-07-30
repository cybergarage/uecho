/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MESSAGE_H_
#define _UECHO_MESSAGE_H_

#include <stdbool.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/object.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoMessage
{
    uEchoObject SEOJ;
    uEchoObject DEOJ;
    byte ESV;
    byte OPC;
} uEchoMessage;

/****************************************
 * Function
 ****************************************/

uEchoMessage *uecho_message_new();
void uecho_message_delete(uEchoMessage *dev);
void uecho_message_clear(uEchoMessage *dev);
#define uecho_message_next(dev) (uEchoMessage *)uecho_list_next((uEchoList *)dev)
#define uecho_message_remove(dev) uecho_list_remove((uEchoList *)dev)

bool uecho_message_start(uEchoMessage *dev);
bool uecho_message_stop(uEchoMessage *dev);
bool uecho_message_isrunning(uEchoMessage *dev);
	
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
