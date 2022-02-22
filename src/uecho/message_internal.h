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

#include <uecho/property_internal.h>

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

typedef struct _uEchoMessage {
  byte EHD1;
  byte EHD2;
  byte TID[uEchoTIDSize];
  byte SEOJ[uEchoEOJSize];
  byte DEOJ[uEchoEOJSize];
  int ESV;
  byte OPC;
  byte OPCSet;
  byte OPCGet;
  uEchoProperty** EP;
  uEchoProperty** EPSet;
  uEchoProperty** EPGet;
  byte* bytes;

  char* srcAddr;
} uEchoMessage;

/****************************************
 * Header
 ****************************************/

#include <uecho/message.h>

/****************************************
 * Function
 ****************************************/

bool uecho_message_requestesv2responseesv(uEchoEsv reqEsv, uEchoEsv* resEsv);
bool uecho_message_requestesv2errorresponseesv(uEchoEsv reqEsv, uEchoEsv* resEsv);

bool uecho_message_parsepacket(uEchoMessage* msg, uEchoDatagramPacket* dgmPkt);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MESSAGE_INTERNAL_H_ */
