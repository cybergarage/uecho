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

#include <uecho/frame/option.h>
#include <uecho/net/socket.h>
#include <uecho/object_internal.h>
#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/thread.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoServerOptionDisableUdpServer = 0x01,
};

/****************************************
 * Data Type
 ****************************************/

// UDP Server

typedef struct UEchoUdpServer {
  UECHO_LIST_STRUCT_MEMBERS

  uEchoSocket* socket;
  uEchoThread* thread;
  void (*msg_listener)(struct UEchoUdpServer*, uEchoMessage*); /* uEchoUdpServerMessageListener */
  void* user_data;
} uEchoUdpServer, uEchoUdpServerList;

typedef void (*uEchoUdpServerMessageListener)(uEchoUdpServer*, uEchoMessage*);

// Multicast Server

typedef struct UEchoMcastServer {
  UECHO_LIST_STRUCT_MEMBERS

  uEchoSocket* socket;
  uEchoThread* thread;
  void (*msg_listener)(struct UEchoMcastServer*, uEchoMessage*); /* uEchoMcastServerMessageListener */
  void* user_data;
} uEchoMcastServer, uEchoMcastServerList;

typedef void (*uEchoMcastServerMessageListener)(uEchoMcastServer*, uEchoMessage*);

// Server

typedef struct UEchoServer {
  uEchoUdpServerList* udp_servers;
  uEchoMcastServerList* mcast_servers;
  void (*msg_listener)(struct UEchoServer*, uEchoMessage*); /* uEchoServerMessageListener */
  void* user_data;
  uEchoOption option;
} uEchoServer;

typedef void (*uEchoServerMessageListener)(uEchoServer*, uEchoMessage*);

/****************************************
 * Function
 ****************************************/

// Server

uEchoServer* uecho_server_new(void);
bool uecho_server_delete(uEchoServer* server);

void uecho_server_setmessagelistener(uEchoServer* server, uEchoServerMessageListener listener);
void uecho_server_setuserdata(uEchoServer* server, void* data);
void* uecho_server_getuserdata(uEchoServer* server);

bool uecho_server_performlistener(uEchoServer* server, uEchoMessage* msg);

bool uecho_server_start(uEchoServer* server);
bool uecho_server_stop(uEchoServer* server);
bool uecho_server_isrunning(uEchoServer* server);

const char * uecho_server_getaddress(uEchoServer* server);

bool uecho_server_postannounce(uEchoServer* server, byte* msg, size_t msg_len);
bool uecho_server_postresponse(uEchoServer* server, const char* addr, byte* msg, size_t msg_len);

#define uecho_server_setoption(server, value) (server->option = value)
#define uecho_server_isoptionenabled(server, value) (server->option & value)
#define uecho_server_isudpserverenabled(ctrl) (!uecho_server_isoptionenabled(ctrl, uEchoServerOptionDisableUdpServer))

bool uecho_server_isboundaddress(uEchoServer* server, const char* addr);

// UDP Server

uEchoUdpServer* uecho_udp_server_new(void);
bool uecho_udp_server_delete(uEchoUdpServer* server);

#define uecho_udp_getsocket(server) (server->socket)

#define uecho_udp_server_next(server) (uEchoUdpServer*)uecho_list_next((uEchoList*)server)
#define uecho_udp_server_remove(server) uecho_list_remove((uEchoList*)server)

void uecho_udp_server_setmessagelistener(uEchoUdpServer* server, uEchoUdpServerMessageListener listener);
void uecho_udp_server_setuserdata(uEchoUdpServer* server, void* data);
void* uecho_udp_server_getuserdata(uEchoUdpServer* server);

bool uecho_udp_server_performlistener(uEchoUdpServer* server, uEchoMessage* msg);

bool uecho_udp_server_open(uEchoUdpServer* server, const char* bind_addr);
bool uecho_udp_server_close(uEchoUdpServer* server);

const char * uecho_udp_server_getaddress(uEchoUdpServer* server);
bool uecho_udp_serverlist_sendto(uEchoUdpServerList* servers, const char* addr, byte* msg, size_t msg_len);

bool uecho_udp_server_start(uEchoUdpServer* server);
bool uecho_udp_server_stop(uEchoUdpServer* server);
bool uecho_udp_server_isrunning(uEchoUdpServer* server);

// Multicast Server

uEchoMcastServer* uecho_mcast_server_new(void);
bool uecho_mcast_server_delete(uEchoMcastServer* server);

#define uecho_mcast_getsocket(server) (server->socket)

#define uecho_mcast_server_next(server) (uEchoMcastServer*)uecho_list_next((uEchoList*)server)
#define uecho_mcast_server_remove(server) uecho_list_remove((uEchoList*)server)

void uecho_mcast_server_setmessagelistener(uEchoMcastServer* server, uEchoMcastServerMessageListener listener);
void uecho_mcast_server_setuserdata(uEchoMcastServer* server, void* data);
void* uecho_mcast_server_getuserdata(uEchoMcastServer* server);

bool uecho_mcast_server_performlistener(uEchoMcastServer* server, uEchoMessage* msg);

bool uecho_mcast_server_open(uEchoMcastServer* server, const char* bind_addr);
bool uecho_mcast_server_close(uEchoMcastServer* server);
bool uecho_mcast_server_isopened(uEchoMcastServer* server);

const char * uecho_mcast_server_getaddress(uEchoMcastServer* server);

bool uecho_mcast_server_start(uEchoMcastServer* server);
bool uecho_mcast_server_stop(uEchoMcastServer* server);
bool uecho_mcast_server_isrunning(uEchoMcastServer* server);

bool uecho_mcast_server_post(uEchoMcastServer* server, const byte* msg, size_t msg_len);

/****************************************
 * Listener
 ****************************************/

void uecho_udp_server_msglistener(uEchoUdpServer* server, uEchoMessage* msg);
void uecho_mcast_server_msglistener(uEchoMcastServer* server, uEchoMessage* msg);

/****************************************
 * Function (ServerList)
 ****************************************/

// UDP Server List

uEchoUdpServerList* uecho_udp_serverlist_new(void);
void uecho_udp_serverlist_delete(uEchoUdpServerList* servers);
bool uecho_udp_serverlist_open(uEchoUdpServerList* servers);
bool uecho_udp_serverlist_close(uEchoUdpServerList* servers);
bool uecho_udp_serverlist_start(uEchoUdpServerList* servers);
bool uecho_udp_serverlist_stop(uEchoUdpServerList* servers);
bool uecho_udp_serverlist_isrunning(uEchoUdpServerList* servers);
void uecho_udp_serverlist_setmessagelistener(uEchoUdpServerList* servers, uEchoUdpServerMessageListener listener);
void uecho_udp_serverlist_setuserdata(uEchoUdpServerList* servers, void* data);
bool uecho_udp_serverlist_post(uEchoMcastServerList* servers, char* addr);
bool uecho_udp_serverlist_isboundaddress(uEchoUdpServerList* servers, const char* addr);

#define uecho_udp_serverlist_clear(servers) uecho_list_clear((uEchoList*)servers, (UECHO_LIST_DESTRUCTORFUNC)uecho_udp_server_delete)
#define uecho_udp_serverlist_size(servers) uecho_list_size((uEchoList*)servers)
#define uecho_udp_serverlist_gets(servers) (uEchoUdpServer*)uecho_list_next((uEchoList*)servers)
#define uecho_udp_serverlist_add(servers, server) uecho_list_add((uEchoList*)servers, (uEchoList*)server)

// Multicast Server List

uEchoMcastServerList* uecho_mcast_serverlist_new(void);
void uecho_mcast_serverlist_delete(uEchoMcastServerList* servers);
bool uecho_mcast_serverlist_open(uEchoMcastServerList* servers);
bool uecho_mcast_serverlist_close(uEchoMcastServerList* servers);
bool uecho_mcast_serverlist_start(uEchoMcastServerList* servers);
bool uecho_mcast_serverlist_stop(uEchoMcastServerList* servers);
bool uecho_mcast_serverlist_isrunning(uEchoMcastServerList* servers);
void uecho_mcast_serverlist_setmessagelistener(uEchoMcastServerList* servers, uEchoMcastServerMessageListener listener);
void uecho_mcast_serverlist_setuserdata(uEchoMcastServerList* servers, void* data);
bool uecho_mcast_serverlist_post(uEchoMcastServerList* servers, const byte* msg, size_t msg_len);
bool uecho_mcast_serverlist_isboundaddress(uEchoMcastServerList* servers, const char* addr);

#define uecho_mcast_serverlist_clear(servers) uecho_list_clear((uEchoList*)servers, (UECHO_LIST_DESTRUCTORFUNC)uecho_mcast_server_delete)
#define uecho_mcast_serverlist_size(servers) uecho_list_size((uEchoList*)servers)
#define uecho_mcast_serverlist_gets(servers) (uEchoMcastServer*)uecho_list_next((uEchoList*)servers)
#define uecho_mcast_serverlist_add(servers, server) uecho_list_add((uEchoList*)servers, (uEchoList*)server)

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
