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
* uecho_message_packet_debug
****************************************/

void uecho_message_packet_debug(const char* addr, const byte* msgBytes, size_t msgLen)
{
  char buf[256];
  size_t offset;
  size_t n;

  if (msgLen <= 0)
    return;

  snprintf(buf, sizeof(buf), "%s -> ", addr);
  offset = strlen(buf);
  for (n = 0; n < msgLen; n++) {
    snprintf((buf + offset), (sizeof(buf) - offset), "%02X", msgBytes[n]);
    offset += 2;
  }

  uecho_log_debug(buf);
}

/****************************************
* uecho_message_debug
****************************************/

void uecho_message_debug(uEchoMessage* msg)
{
  if (!msg)
    return;

  return uecho_message_packet_debug(uecho_message_getsourceaddress(msg), uecho_message_getbytes(msg), uecho_message_size(msg));
}
