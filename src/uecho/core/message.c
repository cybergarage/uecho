/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/message.h>

/****************************************
* uecho_message_new
****************************************/

uEchoMessage *uecho_message_new()
{
	uEchoMessage *msg;

	msg = (uEchoMessage *)malloc(sizeof(uEchoMessage));

    if (!msg)
        return NULL;

  uecho_message_setehd1(msg, UECHO_EHD1);
  uecho_message_setehd2(msg, UECHO_EHD2);

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
 * uecho_message_settid
 ****************************************/

void uecho_message_settid(uEchoMessage *msg, int val) {
  
}

/****************************************
 * uecho_message_gettid
 ****************************************/

int uecho_message_gettid(uEchoMessage *msg) {
  return 0;
}
