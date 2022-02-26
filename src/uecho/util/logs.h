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

#include <uecho/message_internal.h>

void uecho_message_debug(uEchoMessage* msg);
void uecho_net_datagram_packet_debug(uEchoDatagramPacket* dpkt);
void uecho_net_packet_debug(const char* from_addr, const char* to_addr, const byte* msg_bytes, size_t msg_len);

#endif /* _UECHO_LOG_FUNC_H_ */
