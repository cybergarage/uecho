/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_LOG_FUNC_H_
#define _UECHO_LOG_FUNC_H_

#include <uecho/_message.h>

#define UECHO_LOG_NET_PREFIX_SEND "S"
#define UECHO_LOG_NET_PREFIX_RECV "R"

void uecho_net_socket_error(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len);
void uecho_net_socket_debug(const char* prefix, const char* from_addr, const char* to_addr, const byte* pkt_bytes, size_t pkt_len);

void uecho_net_datagram_packet_error(const char* prefix, uEchoDatagramPacket* dpkt);
void uecho_net_datagram_packet_debug(const char* prefix, uEchoDatagramPacket* dpkt);

void uecho_message_error(const char* prefix, uEchoMessage* msg);

#endif /* _UECHO_LOG_FUNC_H_ */
