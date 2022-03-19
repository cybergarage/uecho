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
#include <uecho/util/list.h>
#include <uecho/util/thread.h>
#include <uecho/net/socket.h>
#include <uecho/frame/option.h>
#include <uecho/frame/observer.h>

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
  uEchoMessageObserverManager* msg_mgr;
} uEchoUdpServer, uEchoUdpServerList;

// Multicast Server

typedef struct UEchoMcastServer {
  UECHO_LIST_STRUCT_MEMBERS

  uEchoSocket* socket;
  uEchoThread* thread;
  uEchoMessageObserverManager* msg_mgr;
} uEchoMcastServer, uEchoMcastServerList;

// Server

typedef struct UEchoServer {
  uEchoUdpServerList* udp_servers;
  uEchoMcastServerList* mcast_servers;
  uEchoMessageObserverManager* msg_mgr;
  uEchoOption option;
} uEchoServer;

/****************************************
 * Function
 ****************************************/

// Server

uEchoServer* uecho_server_new(void);
bool uecho_server_delete(uEchoServer* server);

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

bool uecho_server_addobserver(uEchoServer* server, void *obj, uEchoMessageHandler handler);

// UDP Server

uEchoUdpServer* uecho_udp_server_new(void);
bool uecho_udp_server_delete(uEchoUdpServer* server);

#define uecho_udp_getsocket(server) (server->socket)

#define uecho_udp_server_next(server) (uEchoUdpServer*)uecho_list_next((uEchoList*)server)
#define uecho_udp_server_remove(server) uecho_list_remove((uEchoList*)server)

bool uecho_udp_server_open(uEchoUdpServer* server, const char* bind_addr);
bool uecho_udp_server_close(uEchoUdpServer* server);

const char * uecho_udp_server_getaddress(uEchoUdpServer* server);
bool uecho_udp_serverlist_sendto(uEchoUdpServerList* servers, const char* addr, byte* msg, size_t msg_len);

bool uecho_udp_server_start(uEchoUdpServer* server);
bool uecho_udp_server_stop(uEchoUdpServer* server);
bool uecho_udp_server_isrunning(uEchoUdpServer* server);

bool uecho_udp_server_addobserver(uEchoUdpServer* server, void *obj, uEchoMessageHandler handler);

// Multicast Server

uEchoMcastServer* uecho_mcast_server_new(void);
bool uecho_mcast_server_delete(uEchoMcastServer* server);

#define uecho_mcast_getsocket(server) (server->socket)

#define uecho_mcast_server_next(server) (uEchoMcastServer*)uecho_list_next((uEchoList*)server)
#define uecho_mcast_server_remove(server) uecho_list_remove((uEchoList*)server)

bool uecho_mcast_server_open(uEchoMcastServer* server, const char* bind_addr);
bool uecho_mcast_server_close(uEchoMcastServer* server);
bool uecho_mcast_server_isopened(uEchoMcastServer* server);

const char * uecho_mcast_server_getaddress(uEchoMcastServer* server);

bool uecho_mcast_server_start(uEchoMcastServer* server);
bool uecho_mcast_server_stop(uEchoMcastServer* server);
bool uecho_mcast_server_isrunning(uEchoMcastServer* server);

bool uecho_mcast_server_post(uEchoMcastServer* server, const byte* msg, size_t msg_len);

bool uecho_mcast_server_addobserver(uEchoMcastServer* server, void *obj, uEchoMessageHandler handler);

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
