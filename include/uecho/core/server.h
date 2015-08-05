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

// UDP Server

typedef struct _uEchoUdpServer {
  uEchoSocket *socket;
  uEchoThread *thread;
  void (*msgListener)(struct _uEchoUdpServer *, uEchoMessage *); /* uEchoUdpServerMessageListener */
} uEchoUdpServer;

typedef void (*uEchoUdpServerMessageListener)(uEchoUdpServer *, uEchoMessage *);
  
// Multicast Server

typedef struct _uEchoMcastServer {
  uEchoSocket *socket;
  uEchoThread *thread;
  void (*msgListener)(struct _uEchoMcastServer *, uEchoMessage *); /* uEchoMcastServerMessageListener */
} uEchoMcastServer;

typedef void (*uEchoMcastServerMessageListener)(uEchoUdpServer *, uEchoMessage *);

// Server

typedef struct _uEchoServer {
  uEchoUdpServer   *udpServer;
  uEchoMcastServer *mcastServer;
  void (*msgListener)(struct _uEchoServer *, uEchoMessage *); /* uEchoServerMessageListener */
} uEchoServer;

typedef void (*uEchoServerMessageListener)(uEchoServer *, uEchoMessage *);
  
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

/****************************************
 * Macro
 ****************************************/
  
#if defined(C99)
  
inline void uecho_server_setmessagelistener(uEchoServer *server, uEchoServerMessageListener listener) {server->msgListener = listener;}
inline void uecho_udp_server_setmessagelistener(uEchoUdpServer *server, uEchoUdpServerMessageListener listener) {server->msgListener = listener;}
inline void uecho_mcast_server_setmessagelistener(uEchoMcastServer *server, uEchoMcastServerMessageListener listener) {server->msgListener = listener;}
  
#else
  
#define uecho_server_setmessagelistener(server, listener) (server->msgListener = listener)
#define uecho_udp_server_setmessagelistener(server, listener) (server->msgListener = listener)
#define uecho_mcast_server_setmessagelistener(server, listener) (server->msgListener = listener)
  
#endif
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
