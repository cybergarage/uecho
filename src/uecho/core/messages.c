/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/message.h>

/****************************************
* uecho_message_search_new
****************************************/

uEchoMessage *uecho_message_search_new()
{
	uEchoMessage *msg;

	msg = uecho_message_new();

    if (!msg)
        return NULL;
	
	return msg;
}
