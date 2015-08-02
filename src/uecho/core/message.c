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
#include <uecho/net/socket.h>

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

bool uecho_message_settid(uEchoMessage *msg, uEchoTID val) {
  if (UECHO_TID_MAX < val) {
    val %= UECHO_TID_MAX;
  }
  uint16_t nval = htons(val);
  msg->TID[0] = (nval & 0xFF00) >> 8;
  msg->TID[1] = nval & 0x00FF;
  return true;
}

/****************************************
 * uecho_message_gettid
 ****************************************/

uEchoTID uecho_message_gettid(uEchoMessage *msg) {
  uint16_t nval = (msg->TID[0] << 8) + msg->TID[1];
  return ntohs(nval);
}
