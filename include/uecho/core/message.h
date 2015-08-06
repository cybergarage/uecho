/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MESSAGE_H_
#define _UECHO_MESSAGE_H_

#include <uecho/typedef.h>
#include <uecho/const.h>
#include <uecho/object.h>
#include <uecho/property.h>
#include <uecho/net/socket.h>

#ifdef  __cplusplus
extern "C" {
#endif


/****************************************
 * Constant
****************************************/

enum {
  uEchoMessageMinLen = (1 + 1 + 2 + 3 + 3 + 1 + 1),
  uEchoEhd1 = 0x10,
  uEchoEhd2 = 0x81,
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
} uEchoEsvType;

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoMessage
{
  byte EHD1;
  byte EHD2;
  byte TID[2];
  uEchoObject *SEOJ;
  uEchoObject *DEOJ;
  uEchoEsvType ESV;
  byte OPC;
  uEchoProperty **EP;
} uEchoMessage;

/****************************************
 * Function
 ****************************************/

uEchoMessage *uecho_message_new();
void uecho_message_delete(uEchoMessage *msg);

bool uecho_message_parse(uEchoMessage *msg, const byte *data, size_t dataLen);
bool uecho_message_parsepacket(uEchoMessage *msg, uEchoDatagramPacket *dgmPkt);

bool uecho_message_settid(uEchoMessage *msg, uEchoTID val);
uEchoTID uecho_message_gettid(uEchoMessage *msg);

bool uecho_message_setopc(uEchoMessage *msg, byte val);
byte uecho_message_getopc(uEchoMessage *msg);

uEchoProperty *uecho_message_getproperty(uEchoMessage *msg, size_t n);
  
void uecho_message_clear(uEchoMessage *msg);

/****************************************
 * Macro
 ****************************************/

#if defined(C99)

inline void uecho_message_setehd1(uEchoMessage *msg, byte val) {msg->EHD1 = val;}
inline byte uecho_message_getehd1(uEchoMessage *msg) {return msg->EHD1;}
    
inline uEchoObject *uecho_message_getsourceobject(uEchoMessage *msg) {return msg->SEOJ;}
inline uEchoObject *uecho_message_getdestinationobject(uEchoMessage *msg) {return msg->DEOJ;}

inline void uecho_message_setehd2(uEchoMessage *msg, byte val) {msg->EHD2 = val;}
inline byte uecho_message_getehd2(uEchoMessage *msg) {return msg->EHD2;}

inline void uecho_message_setesv(uEchoMessage *msg, uEchoEsvType val) {msg->ESV = val;}
inline uEchoEsvType uecho_message_getesv(uEchoMessage *msg) {return msg->ESV;}

#else

#define uecho_message_setehd1(msg, val) (msg->EHD1 = val)
#define uecho_message_getehd1(msg) (msg->EHD1)
    
#define uecho_message_getsourceobject(msg) (msg->SEOJ)
#define uecho_message_getdestinationobject(msg) (msg->DEOJ)
    
#define uecho_message_setehd2(msg, val) (msg->EHD2 = val)
#define uecho_message_getehd2(msg) (msg->EHD2)
    
#define uecho_message_setesv(msg, val) (msg->ESV = val)
#define uecho_message_getesv(msg) (msg->ESV)
    
#endif

/****************************************
* Function
****************************************/

uEchoMessage *uecho_message_search_new();

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
