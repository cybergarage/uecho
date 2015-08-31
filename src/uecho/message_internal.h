/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MESSAGE_INTERNAL_H_
#define _UECHO_MESSAGE_INTERNAL_H_

#include <uecho/typedef.h>
#include <uecho/const.h>
#include <uecho/net/socket.h>
#include <uecho/property_internal.h>

#ifdef  __cplusplus
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

typedef struct _uEchoMessage
{
  byte EHD1;
  byte EHD2;
  byte TID[uEchoTIDSize];
  byte SEOJ[uEchoEOJSize];
  byte DEOJ[uEchoEOJSize];
  int ESV;
  byte OPC;
  uEchoProperty **EP;
  byte *bytes;

  char *srcAddr;
} uEchoMessage;

/****************************************
 * Header
 ****************************************/
  
#include <uecho/message.h>
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MESSAGE_INTERNAL_H_ */
