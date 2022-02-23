/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/esv.h>

/****************************************
 * uecho_esv_iswriterequest
 ****************************************/

bool uecho_esv_iswriterequest(int esv)
{
  if ((esv == uEchoEsvWriteRequest) || (esv == uEchoEsvWriteRequestResponseRequired) || (esv == uEchoEsvWriteReadRequest))
    return true;
  return false;
}

/****************************************
 * uecho_esv_isreadrequest
 ****************************************/

bool uecho_esv_isreadrequest(int esv)
{
  if ((esv == uEchoEsvReadRequest) || (esv == uEchoEsvWriteReadRequest))
    return true;
  return false;
}

/****************************************
 * uecho_esv_isnotifyrequest
 ****************************************/

bool uecho_esv_isnotifyrequest(int esv)
{
  if ((esv == uEchoEsvNotificationRequest) || (esv == uEchoEsvNotificationResponseRequired))
    return true;
  return false;
}

/****************************************
 * uecho_esv_iswriteresponse
 ****************************************/

bool uecho_esv_iswriteresponse(int esv)
{
  if ((esv == uEchoEsvWriteResponse) || (esv == uEchoEsvWriteReadResponse))
    return true;
  return false;
}

/****************************************
 * uecho_esv_isreadresponse
 ****************************************/

bool uecho_esv_isreadresponse(int esv)
{
  if ((esv == uEchoEsvReadResponse) || (esv == uEchoEsvWriteReadResponse))
    return true;
  return false;
}

/****************************************
 * uecho_esv_isnotifyresponse
 ****************************************/

bool uecho_esv_isnotifyresponse(int esv)
{
  if ((esv == uEchoEsvNotification) || (esv == uEchoEsvNotificationResponse))
    return true;
  return false;
}
