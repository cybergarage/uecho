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

    uecho_message_settid(msg, 0);
    
    msg->SEOJ = uecho_object_new();
    msg->DEOJ = uecho_object_new();
    
    uecho_message_setesv(msg, 0);

    uecho_message_setopc(msg, 0);
    
    return msg;
}

/****************************************
* uecho_message_delete
****************************************/

void uecho_message_delete(uEchoMessage *msg)
{
    uecho_object_delete(msg->SEOJ);
    uecho_object_delete(msg->DEOJ);
    
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

/****************************************
 * uecho_message_gettid
 ****************************************/

bool uecho_message_setopc(uEchoMessage *msg, byte val) {

    return true;
}

/****************************************
 * uecho_message_getopc
 ****************************************/

byte uecho_message_getopc(uEchoMessage *msg) {
    return msg->OPC;
}

