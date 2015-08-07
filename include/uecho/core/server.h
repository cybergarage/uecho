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
#include <uecho/util/list.h>
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
  bool headFlag;
  struct _uEchoUdpServer *prev;
  struct _uEchoUdpServer *next;
  
  uEchoSocket *socket;
  uEchoThread *thread;
  void (*msgListener)(struct _uEchoUdpServer *, uEchoMessage *); /* uEchoUdpServerMessageListener */
  void *userData;
} uEchoUdpServer, uEchoUdpServerList;

typedef void (*uEchoUdpServerMessageListener)(uEchoUdpServer *, uEchoMessage *);
  
// Multicast Server

typedef struct _uEchoMcastServer {
  bool headFlag;
  struct _uEchoMcastServer *prev;
  struct _uEchoMcastServer *next;
  
  uEchoSocket *socket;
  uEchoThread *thread;
  void (*msgListener)(struct _uEchoMcastServer *, uEchoMessage *); /* uEchoMcastServerMessageListener */
  void *userData;
} uEchoMcastServer, uEchoMcastServerList;

typedef void (*uEchoMcastServerMessageListener)(uEchoUdpServer *, uEchoMessage *);

// Server

typedef struct _uEchoServer {
  uEchoUdpServer   *udpServer;
  uEchoMcastServer *mcastServer;
  void (*msgListener)(struct _uEchoServer *, uEchoMessage *); /* uEchoServerMessageListener */
  void *userData;
} uEchoServer;

typedef void (*uEchoServerMessageListener)(uEchoServer *, uEchoMessage *);
  
/****************************************
 * Function
 ****************************************/

// Server

uEchoServer *uecho_server_new();
void uecho_server_delete(uEchoServer *server);

bool uecho_server_performlistener(uEchoServer *server, uEchoMessage *msg);

bool uecho_server_start(uEchoServer *server);
bool uecho_server_stop(uEchoServer *server);
bool uecho_server_isrunning(uEchoServer *server);

// UDP Server
  
uEchoUdpServer *uecho_udp_server_new();
void uecho_udp_server_delete(uEchoUdpServer *server);
  
bool uecho_udp_server_performlistener(uEchoUdpServer *server, uEchoMessage *msg);

bool uecho_udp_server_start(uEchoUdpServer *server);
bool uecho_udp_server_stop(uEchoUdpServer *server);
bool uecho_udp_server_isrunning(uEchoUdpServer *server);

// Multicast Server
  
uEchoMcastServer *uecho_mcast_server_new();
void uecho_mcast_server_delete(uEchoMcastServer *server);
  
bool uecho_mcast_server_performlistener(uEchoMcastServer *server, uEchoMessage *msg);

bool uecho_mcast_server_start(uEchoMcastServer *server);
bool uecho_mcast_server_stop(uEchoMcastServer *server);
bool uecho_mcast_server_isrunning(uEchoMcastServer *server);

/****************************************
 * Function (ServerList)
 ****************************************/
  
uEchoUdpServerList *uecho_udp_serverlist_new();
void uecho_udp_serverlist_delete(uEchoUdpServerList *servers);

#define uecho_udp_serverlist_clear(servers) uecho_list_clear((uEchoList *)servers, (UECHO_LIST_DESTRUCTORFUNC)uecho_udp_server_delete)
#define uecho_udp_serverlist_size(servers) uecho_list_size((uEchoList *)servers)
#define uecho_udp_serverlist_gets(servers) (uEchoNetworkInterface *)uecho_list_next((uEchoList *)servers)
#define uecho_udp_serverlist_add(servers,server) uecho_list_add((uEchoList *)servers, (uEchoList *)server)

uEchoMcastServerList *uecho_mcast_serverlist_new();
void uecho_mcast_serverlist_delete(uEchoMcastServerList *servers);

#define uecho_mcast_serverlist_clear(servers) uecho_list_clear((uEchoList *)servers, (UECHO_LIST_DESTRUCTORFUNC)uecho_mcast_server_delete)
#define uecho_mcast_serverlist_size(servers) uecho_list_size((uEchoList *)servers)
#define uecho_mcast_serverlist_gets(servers) (uEchoNetworkInterface *)uecho_list_next((uEchoList *)servers)
#define uecho_mcast_serverlist_add(servers,server) uecho_list_add((uEchoList *)servers, (uEchoList *)server)

/****************************************
 * Macro
 ****************************************/
  
#if defined(C99)
  
// Server

inline void uecho_server_setmessagelistener(uEchoServer *server, uEchoServerMessageListener listener) {server->msgListener = listener;}
inline void uecho_server_setuserdata(uEchoServer *server, void *data) {server->userData = data;}
inline void *uecho_server_getuserdata(uEchoServer *server) {return server->userData;}
  
// UDP Server

inline void uecho_udp_server_setmessagelistener(uEchoUdpServer *server, uEchoUdpServerMessageListener listener) {server->msgListener = listener;}
inline void uecho_udp_server_setuserdata(uEchoUdpServer *server, void *data) {server->userData = data;}
inline void *uecho_udp_server_getuserdata(uEchoUdpServer *server) {return server->userData;}

  
// Multicast Server

inline void uecho_mcast_server_setmessagelistener(uEchoMcastServer *server, uEchoMcastServerMessageListener listener) {server->msgListener = listener;}
inline void uecho_mcast_server_setuserdata(uEchoMcastServer *server, void *data) {server->userData = data;}
inline void *uecho_mcast_server_getuserdata(uEchoMcastServer *server) {return server->userData;}
  
#else
  
// Server

#define uecho_server_setmessagelistener(server, listener) (server->msgListener = listener)
#define uecho_server_setuserdata(server, data) (server->userData = data)
#define uecho_server_getuserdata(server) (server->userData)

// UDP Server

#define uecho_udp_server_setmessagelistener(server, listener) (server->msgListener = listener)
#define uecho_udp_server_setuserdata(server, data) (server->userData = data)
#define uecho_udp_server_getuserdata(server) (server->userData)

// Multicast Server

#define uecho_mcast_server_setmessagelistener(server, listener) (server->msgListener = listener)
#define uecho_mcast_server_setuserdata(server, data) (server->userData = data)
#define uecho_mcast_server_getuserdata(server) (server->userData)
  
#endif
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
