/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MESSAGE_INTERNAL_H_
#define _UECHO_MESSAGE_INTERNAL_H_

#include <uecho/_property.h>

#include <uecho/const.h>
#include <uecho/net/socket.h>
#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoMessageHeaderLen = (1 + 1 + 2),
  uEchoMessageMinLen = (uEchoMessageHeaderLen + 3 + 3 + 1 + 1),
  uEchoEhd1 = 0x10,
  uEchoEhd2 = 0x81,
  uEchoTIDSize = 2,
  uEchoEOJSize = 3,
};

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoMessage {
  byte ehd1;
  byte ehd2;
  byte tid[uEchoTIDSize];
  byte seoj[uEchoEOJSize];
  byte deoj[uEchoEOJSize];
  byte esv;
  byte opc;
  byte opc_set;
  byte opc_get;
  uEchoProperty** ep;
  uEchoProperty** ep_set;
  uEchoProperty** ep_get;
  byte* bytes;

  char* from_addr;
  char* to_addr;
} uEchoMessage;

/****************************************
 * Header
 ****************************************/

#include <uecho/message.h>

/****************************************
 * Function
 ****************************************/

void uecho_message_setehd1(uEchoMessage* msg, byte val);
void uecho_message_setehd2(uEchoMessage* msg, byte val);

bool uecho_message_setopc(uEchoMessage* msg, byte val);
bool uecho_message_setopcset(uEchoMessage* msg, byte val);
bool uecho_message_setopcget(uEchoMessage* msg, byte val);

bool uecho_message_requestesv2responseesv(uEchoEsv req_esv, uEchoEsv* res_esv);
bool uecho_message_requestesv2errorresponseesv(uEchoEsv req_esv, uEchoEsv* res_esv);

bool uecho_message_parsepacket(uEchoMessage* msg, uEchoDatagramPacket* dgm_pkt);

bool uecho_message_isreadwritemessage(uEchoMessage* msg);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MESSAGE_INTERNAL_H_ */
