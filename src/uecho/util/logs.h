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

void uecho_message_packet_debug(const char* addr, const byte* msg, size_t msgLen);
void uecho_socket_datagram_packet_debug(uEchoDatagramPacket* dpkt);
void uecho_message_debug(uEchoMessage* msg);

#endif /* _UECHO_LOG_FUNC_H_ */
