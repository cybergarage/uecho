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

#include <uecho/typedef.h>
#include <uecho/const.h>
#include <uecho/net/socket.h>
#include <uecho/property.h>

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

typedef enum {
  uEchoEsvWriteRequest = 0x60,
  uEchoEsvWriteRequestResponseRequired = 0x61,
  uEchoEsvReadRequest = 0x62,
  uEchoEsvNotificationRequest = 0x63,
  uEchoEsvWriteReadRequest = 0x6E,

  uEchoEsvWriteResponse = 0x71,
  uEchoEsvReadResponse = 0x72,
  uEchoEsvNotification = 0x73,
  uEchoEsvNotificationResponseRequired = 0x74,
  uEchoEsvNotificationResponse = 0x7A,
  uEchoEsvWriteReadResponse = 0x7E,
} uEchoEsv;

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
  uEchoEsv ESV;
  byte OPC;
  uEchoProperty **EP;
  byte *bytes;

  char *srcAddr;
} uEchoMessage;

/****************************************
 * Function
 ****************************************/

uEchoMessage *uecho_message_new(void);
void uecho_message_delete(uEchoMessage *msg);
void uecho_message_clear(uEchoMessage *msg);

bool uecho_message_parse(uEchoMessage *msg, const byte *data, size_t dataLen);
bool uecho_message_parsepacket(uEchoMessage *msg, uEchoDatagramPacket *dgmPkt);

bool uecho_message_settid(uEchoMessage *msg, uEchoTID val);
uEchoTID uecho_message_gettid(uEchoMessage *msg);

bool uecho_message_setopc(uEchoMessage *msg, byte val);
byte uecho_message_getopc(uEchoMessage *msg);

void uecho_message_setehd1(uEchoMessage *msg, byte val);
byte uecho_message_getehd1(uEchoMessage *msg);

bool uecho_message_setsourceobjectcode(uEchoMessage *msg, int code);
bool uecho_message_setdestinationobjectcode(uEchoMessage *msg, int code);

int uecho_message_getsourceobjectcode(uEchoMessage *msg);
int uecho_message_getdestinationobjectcode(uEchoMessage *msg);
  
void uecho_message_setehd2(uEchoMessage *msg, byte val);
byte uecho_message_getehd2(uEchoMessage *msg);
  
void uecho_message_setesv(uEchoMessage *msg, uEchoEsv val);
uEchoEsv uecho_message_getesv(uEchoMessage *msg);

bool uecho_message_iswriterequest(uEchoMessage *msg);
bool uecho_message_isreaderequest(uEchoMessage *msg);
bool uecho_message_isnotifyrequest(uEchoMessage *msg);

bool uecho_message_addproperty(uEchoMessage *msg, uEchoProperty *prop);
uEchoProperty *uecho_message_getproperty(uEchoMessage *msg, size_t n);
uEchoProperty *uecho_message_getpropertybycode(uEchoMessage *msg, uEchoPropertyCode code);

size_t uecho_message_size(uEchoMessage *msg);
byte *uecho_message_getbytes(uEchoMessage *msg);

void uecho_message_setsourceaddress(uEchoMessage *msg, const char *addr);
const char *uecho_message_getsourceaddress(uEchoMessage *msg);

uEchoMessage *uecho_message_copy(uEchoMessage *msg);
bool uecho_message_equals(uEchoMessage *msg1, uEchoMessage *msg2);

/****************************************
* Function
****************************************/

uEchoMessage *uecho_message_search_new(void);
bool uecho_message_issearchrequest(uEchoMessage *msg);

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
