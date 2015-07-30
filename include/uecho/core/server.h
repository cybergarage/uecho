/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_SERVER_H_
#define _UECHO_SERVER_H_

#include <uecho/typedef.h>
#include <uecho/net/socket.h>
#include <uecho/util/thread.h>
#include <uecho/object.h>
#include <uecho/core/message.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Server
 ****************************************/

typedef void (*UECHO_MESSAGE_LISTNER)(uEchoMessage *);
    
// UDP Server

typedef struct _uEchoUdpServer {
    uEchoSocket *socket;
    uEchoThread *thread;
    UECHO_MESSAGE_LISTNER listener;
} uEchoUdpServer;

// Multicast Server

typedef struct _uEchoMcastServer {
    uEchoSocket *socket;
    uEchoThread *thread;
    UECHO_MESSAGE_LISTNER listener;
} uEchoMcastServer;

// Server

typedef struct _uEchoServer {
    uEchoUdpServer   *udpServer;
    uEchoMcastServer *mcastServer;
} uEchoServer;

/****************************************
 * Function
 ****************************************/

// Server

uEchoServer *uecho_server_new();
void uecho_server_delete(uEchoServer *server);
void uecho_server_clear(uEchoServer *server);

bool uecho_server_start(uEchoServer *server);
bool uecho_server_stop(uEchoServer *server);
bool uecho_server_isrunning(uEchoServer *server);

// UDP Server
    
uEchoUdpServer *uecho_udp_server_new();
void uecho_udp_server_delete(uEchoUdpServer *server);
void uecho_udp_server_clear(uEchoUdpServer *server);
    
bool uecho_udp_server_start(uEchoUdpServer *server);
bool uecho_udp_server_stop(uEchoUdpServer *server);
bool uecho_udp_server_isrunning(uEchoUdpServer *server);

// Multicast Server
    
uEchoMcastServer *uecho_mcast_server_new();
void uecho_mcast_server_delete(uEchoMcastServer *server);
void uecho_mcast_server_clear(uEchoMcastServer *server);
    
bool uecho_mcast_server_start(uEchoMcastServer *server);
bool uecho_mcast_server_stop(uEchoMcastServer *server);
bool uecho_mcast_server_isrunning(uEchoMcastServer *server);
    
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
