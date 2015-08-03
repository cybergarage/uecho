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

#ifdef  __cplusplus
extern "C" {
#endif


/****************************************
 * Constant
****************************************/

enum {
  uEchoEsvPropertyValueWriteRequest = 0x60,
  uEchoEsvPropertyValueWriteRequestResponseRequired = 0x61,
  uEchoEsvPropertyValueReadRequest = 0x62,
  uEchoEsvPropertyValueNotificationRequest = 0x63,
  uEchoEsvPropertyValueWriteReadRequest = 0x6E,

  uEchoEsvPropertyValueWriteResponse = 0x71,
  uEchoEsvPropertyValueReadResponse = 0x72,
  uEchoEsvPropertyValueNotification = 0x73,
  uEchoEsvPropertyValueNotificationResponseRequired = 0x74,
  uEchoEsvPropertyValueNotificationResponse = 0x7A,
  uEchoEsvPropertyValueWriteReadResponse = 0x7E,
} uEchoEsvPropertyValueType;

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoMessage
{
    byte EHD1;
    byte EHD2;
    byte TID[2];
    uEchoObject SEOJ;
    uEchoObject DEOJ;
    byte ESV;
    byte OPC;
} uEchoMessage;

/****************************************
 * Function
 ****************************************/

uEchoMessage *uecho_message_new();
void uecho_message_delete(uEchoMessage *msg);

#define uecho_message_setehd1(msg, val) (msg->EHD1 = val)
#define uecho_message_getehd1(msg) (msg->EHD1)

#define uecho_message_setehd2(msg, val) (msg->EHD2 = val)
#define uecho_message_getehd2(msg) (msg->EHD2)

bool uecho_message_settid(uEchoMessage *msg, uEchoTID val);
uEchoTID uecho_message_gettid(uEchoMessage *msg);

bool uecho_message_start(uEchoMessage *msg);
bool uecho_message_stop(uEchoMessage *msg);
bool uecho_message_isrunning(uEchoMessage *msg);

/****************************************
* Function
****************************************/

uEchoMessage *uecho_message_search_new();

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
