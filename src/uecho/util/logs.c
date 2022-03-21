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
#include <uecho/util/log_internal.h>
#include <uecho/util/logs.h>

/****************************************
* uecho_net_socket_log
****************************************/

void uecho_net_socket_log(int severity, const char *prefix, const char* from_addr, const char* to_addr, const byte* msg_bytes, size_t msg_len)
{
  char buf[256];
  size_t offset;
  size_t n;

  if (msg_len <= 0)
    return;

  offset = 0;
  if (prefix && (0 < strlen(prefix))) {
    snprintf(buf, sizeof(buf), "%s ", prefix);
    offset = strlen(buf);
  }
  snprintf((buf + offset), (sizeof(buf) - offset), "%-15s -> %-15s ", from_addr, to_addr);
  offset = strlen(buf);
  for (n = 0; n < msg_len; n++) {
    snprintf((buf + offset), (sizeof(buf) - offset), "%02X", msg_bytes[n]);
    offset += 2;
  }

  uecho_log_debug(buf);
}

/****************************************
* uecho_net_socket_*
****************************************/

void uecho_net_socket_debug(const char*prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len)
{
  return uecho_net_socket_log(UECHO_LOG_DEBUG, prefix, from_addr,to_addr, pkt_bytes, pkt_len);
}

void uecho_net_socket_error(const char*prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len)
{
  return uecho_net_socket_log(UECHO_LOG_ERROR, prefix, from_addr,to_addr, pkt_bytes, pkt_len);
}

/****************************************
* uecho_net_datagram_packet_*
****************************************/

void uecho_net_datagram_packet_error(const char*prefix, uEchoDatagramPacket* dpkt)
{
  if (!dpkt)
    return;

  return uecho_net_socket_error(
      prefix,
      uecho_socket_datagram_packet_getremoteaddress(dpkt),
      uecho_socket_datagram_packet_getlocaladdress(dpkt),
      uecho_socket_datagram_packet_getdata(dpkt),
      uecho_socket_datagram_packet_getlength(dpkt));
}

void uecho_net_datagram_packet_debug(const char*prefix, uEchoDatagramPacket* dpkt)
{
  if (!dpkt)
    return;

  return uecho_net_socket_debug(
      prefix,
      uecho_socket_datagram_packet_getremoteaddress(dpkt),
      uecho_socket_datagram_packet_getlocaladdress(dpkt),
      uecho_socket_datagram_packet_getdata(dpkt),
      uecho_socket_datagram_packet_getlength(dpkt));
}

/****************************************
* uecho_message_error
****************************************/

void uecho_message_error(const char*prefix, uEchoMessage* msg)
{
  if (!msg)
    return;

  return uecho_net_socket_log(
      UECHO_LOG_ERROR,
      prefix,
      uecho_message_getsourceaddress(msg),
      uecho_message_getdestinationaddress(msg),
      uecho_message_getbytes(msg),
      uecho_message_size(msg));
}
