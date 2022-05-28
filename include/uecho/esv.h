/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_ESV_H_
#define _UECHO_ESV_H_

#include <uecho/const.h>
#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef byte uEchoEsv;

/****************************************
 * Constant
****************************************/

enum {
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

  uEchoEsvWriteRequestError = 0x50,
  uEchoEsvWriteRequestResponseRequiredError = 0x51,
  uEchoEsvReadRequestError = 0x52,
  uEchoEsvNotificationRequestError = 0x53,
  uEchoEsvWriteReadRequestError = 0x5E,
};

/****************************************
* Function
****************************************/

bool uecho_esv_isnotification(uEchoEsv esv);

bool uecho_esv_iswriterequest(uEchoEsv esv);
bool uecho_esv_isreadrequest(uEchoEsv esv);
bool uecho_esv_isnotifyrequest(uEchoEsv esv);

bool uecho_esv_iswriteresponse(uEchoEsv esv);
bool uecho_esv_isreadresponse(uEchoEsv esv);
bool uecho_esv_isnotifyresponse(uEchoEsv esv);

bool uecho_esv_isrequest(uEchoEsv esv);
bool uecho_esv_isresponse(uEchoEsv esv);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_ESV_H_ */
