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
#include <uecho/util/log.h>
#include <uecho/util/logs.h>

/****************************************
* uecho_message_debug
****************************************/

void uecho_message_debug(uEchoMessage* msg)
{
  if (!msg)
    return;

  return uecho_net_packet_debug(
      uecho_message_getsourceaddress(msg),
      uecho_message_getbytes(msg),
      uecho_message_size(msg));
}

/****************************************
* uecho_net_datagram_packet_debug
****************************************/

void uecho_net_datagram_packet_debug(uEchoDatagramPacket* dpkt)
{
  if (!dpkt)
    return;

  return uecho_net_packet_debug(
      uecho_socket_datagram_packet_getremoteaddress(dpkt),
      uecho_socket_datagram_packet_getdata(dpkt),
      uecho_socket_datagram_packet_getlength(dpkt));
}

/****************************************
* uecho_net_packet_debug
****************************************/

void uecho_net_packet_debug(const char* from_addr, const char* to_addr, const byte* msg_bytes, size_t msg_len)
{
  char buf[256];
  size_t offset;
  size_t n;

  if (msg_len <= 0)
    return;

  snprintf(buf, sizeof(buf), "%s -> %s ", from_addr, to_addr);
  offset = strlen(buf);
  for (n = 0; n < msg_len; n++) {
    snprintf((buf + offset), (sizeof(buf) - offset), "%02X", msg_bytes[n]);
    offset += 2;
  }

  uecho_log_debug(buf);
}
