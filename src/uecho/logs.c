/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdio.h>
#include <string.h>
#include <uecho/logs.h>
#include <uecho/util/log.h>

/****************************************
* uecho_message_delete
****************************************/

void uecho_message_debug(uEchoMessage* msg)
{
  char buf[256];
  size_t offset;
  size_t msgSize;
  byte* msgBytes;
  size_t n;

  if (!msg)
    return;

  msgSize = uecho_message_size(msg);
  if (msgSize <= 0)
    return;

  msgBytes = uecho_message_getbytes(msg);

  snprintf(buf, sizeof(buf), "%s -> ", uecho_message_getsourceaddress(msg));
  offset = strlen(buf);
  for (n = 0; n < msgSize; n++) {
    snprintf((buf + offset), (sizeof(buf) - offset), "%02X", msgBytes[n]);
    offset += 2;
  }

  uecho_log_debug(buf);
}
