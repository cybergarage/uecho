/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
#include <uecho/message.h>

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

typedef void (*uEchoMcastServerMessageListener)(uEchoMcastServer *, uEchoMessage *);

// Server

typedef struct _uEchoServer {
  uEchoUdpServerList   *udpServers;
  uEchoMcastServerList *mcastServers;
  void (*msgListener)(struct _uEchoServer *, uEchoMessage *); /* uEchoServerMessageListener */
  void *userData;
} uEchoServer;

typedef void (*uEchoServerMessageListener)(uEchoServer *, uEchoMessage *);
  
/****************************************
 * Function
 ****************************************/

// Server

uEchoServer *uecho_server_new(void);
void uecho_server_delete(uEchoServer *server);

void uecho_server_setmessagelistener(uEchoServer *server, uEchoServerMessageListener listener);
void uecho_server_setuserdata(uEchoServer *server, void *data);
void *uecho_server_getuserdata(uEchoServer *server);

bool uecho_server_performlistener(uEchoServer *server, uEchoMessage *msg);

bool uecho_server_start(uEchoServer *server);
bool uecho_server_stop(uEchoServer *server);
bool uecho_server_isrunning(uEchoServer *server);

bool uecho_server_postannounce(uEchoServer *server, const byte *msg, size_t msgLen);
bool uecho_server_postresponse(uEchoServer *server, const char *addr, byte *msg, size_t msgLen);

// UDP Server
  
uEchoUdpServer *uecho_udp_server_new(void);
void uecho_udp_server_delete(uEchoUdpServer *server);
  
#define uecho_udp_server_next(netIf) (uEchoUdpServer *)uecho_list_next((uEchoList *)netIf)
#define uecho_udp_server_remove(netIf) uecho_list_remove((uEchoList *)netIf)

void uecho_udp_server_setmessagelistener(uEchoUdpServer *server, uEchoUdpServerMessageListener listener);
void uecho_udp_server_setuserdata(uEchoUdpServer *server, void *data);
void *uecho_udp_server_getuserdata(uEchoUdpServer *server);

bool uecho_udp_server_performlistener(uEchoUdpServer *server, uEchoMessage *msg);

bool uecho_udp_server_open(uEchoUdpServer *server, const char *bindAddr);
bool uecho_udp_server_close(uEchoUdpServer *server);
  
bool uecho_udp_server_start(uEchoUdpServer *server);
bool uecho_udp_server_stop(uEchoUdpServer *server);
bool uecho_udp_server_isrunning(uEchoUdpServer *server);

// Multicast Server
  
uEchoMcastServer *uecho_mcast_server_new(void);
void uecho_mcast_server_delete(uEchoMcastServer *server);

#define uecho_mcast_server_next(netIf) (uEchoMcastServer *)uecho_list_next((uEchoList *)netIf)
#define uecho_mcast_server_remove(netIf) uecho_list_remove((uEchoList *)netIf)

void uecho_mcast_server_setmessagelistener(uEchoMcastServer *server, uEchoMcastServerMessageListener listener);
void uecho_mcast_server_setuserdata(uEchoMcastServer *server, void *data);
void *uecho_mcast_server_getuserdata(uEchoMcastServer *server);

bool uecho_mcast_server_performlistener(uEchoMcastServer *server, uEchoMessage *msg);

bool uecho_mcast_server_open(uEchoMcastServer *server, const char *bindAddr);
bool uecho_mcast_server_close(uEchoMcastServer *server);
bool uecho_mcast_server_isopened(uEchoMcastServer *server);

bool uecho_mcast_server_start(uEchoMcastServer *server);
bool uecho_mcast_server_stop(uEchoMcastServer *server);
bool uecho_mcast_server_isrunning(uEchoMcastServer *server);

bool uecho_mcast_server_post(uEchoMcastServer *server, const byte *msg, size_t msgLen);

/****************************************
 * Function (ServerList)
 ****************************************/

// UDP Server List
  
uEchoUdpServerList *uecho_udp_serverlist_new(void);
void uecho_udp_serverlist_delete(uEchoUdpServerList *servers);
bool uecho_udp_serverlist_open(uEchoUdpServerList *servers);
bool uecho_udp_serverlist_close(uEchoUdpServerList *servers);
bool uecho_udp_serverlist_start(uEchoUdpServerList *servers);
bool uecho_udp_serverlist_stop(uEchoUdpServerList *servers);
void uecho_udp_serverlist_setmessagelistener(uEchoUdpServerList *servers, uEchoUdpServerMessageListener listener);
void uecho_udp_serverlist_setuserdata(uEchoUdpServerList *servers, void *data);

#define uecho_udp_serverlist_clear(servers) uecho_list_clear((uEchoList *)servers, (UECHO_LIST_DESTRUCTORFUNC)uecho_udp_server_delete)
#define uecho_udp_serverlist_size(servers) uecho_list_size((uEchoList *)servers)
#define uecho_udp_serverlist_gets(servers) (uEchoUdpServer *)uecho_list_next((uEchoList *)servers)
#define uecho_udp_serverlist_add(servers,server) uecho_list_add((uEchoList *)servers, (uEchoList *)server)

// Multicast Server List

uEchoMcastServerList *uecho_mcast_serverlist_new(void);
void uecho_mcast_serverlist_delete(uEchoMcastServerList *servers);
bool uecho_mcast_serverlist_open(uEchoMcastServerList *servers);
bool uecho_mcast_serverlist_close(uEchoMcastServerList *servers);
bool uecho_mcast_serverlist_start(uEchoMcastServerList *servers);
bool uecho_mcast_serverlist_stop(uEchoMcastServerList *servers);
void uecho_mcast_serverlist_setmessagelistener(uEchoMcastServerList *servers, uEchoMcastServerMessageListener listener);
void uecho_mcast_serverlist_setuserdata(uEchoMcastServerList *servers, void *data);
bool uecho_mcast_serverlist_post(uEchoMcastServerList *servers, const byte *msg, size_t msgLen);

#define uecho_mcast_serverlist_clear(servers) uecho_list_clear((uEchoList *)servers, (UECHO_LIST_DESTRUCTORFUNC)uecho_mcast_server_delete)
#define uecho_mcast_serverlist_size(servers) uecho_list_size((uEchoList *)servers)
#define uecho_mcast_serverlist_gets(servers) (uEchoMcastServer *)uecho_list_next((uEchoList *)servers)
#define uecho_mcast_serverlist_add(servers,server) uecho_list_add((uEchoList *)servers, (uEchoList *)server)

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
