/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/frame/message.h>

/****************************************
* uecho_message_new
****************************************/

uEchoMessage *uecho_message_new()
{
	uEchoMessage *msg;

	msg = (uEchoMessage *)malloc(sizeof(uEchoMessage));

    if (!msg)
        return NULL;
	
	return msg;
}

/****************************************
* uecho_message_delete
****************************************/

void uecho_message_delete(uEchoMessage *msg)
{
	uecho_list_remove((uEchoList *)msg);
    
	free(msg);
}

/****************************************
* uecho_message_clear
****************************************/

void uecho_message_clear(uEchoMessage *msg)
{
}
