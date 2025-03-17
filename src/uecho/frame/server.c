/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/frame/server.h>

/****************************************
 * uecho_server_new
 ****************************************/

uEchoServer* uecho_server_new(void)
{
  uEchoServer* server;

  server = (uEchoServer*)malloc(sizeof(uEchoServer));

  if (!server)
    return NULL;

  server->udpServers = uecho_udp_serverlist_new();
  server->mcastServers = uecho_mcast_serverlist_new();
  server->msgMgr = uecho_message_observer_manager_new();

  uecho_server_setoption(server, uEchoOptionNone);

  return server;
}

/****************************************
 * uecho_server_delete
 ****************************************/

bool uecho_server_delete(uEchoServer* server)
{
  if (!server)
    return false;

  uecho_udp_serverlist_delete(server->udpServers);
  uecho_mcast_serverlist_delete(server->mcastServers);
  uecho_message_observer_manager_delete(server->msgMgr);

  free(server);

  return true;
}

/****************************************
 * uecho_server_isboundaddress
 ****************************************/

bool uecho_server_isboundaddress(uEchoServer* server, const char* addr)
{
  if (!server)
    return false;

  if (uecho_udp_serverlist_isboundaddress(server->udpServers, addr))
    return true;

  if (uecho_mcast_serverlist_isboundaddress(server->mcastServers, addr))
    return true;

  return false;
}

/****************************************
 * uecho_server_start
 ****************************************/

bool uecho_server_start(uEchoServer* server)
{
  bool allActionsSucceeded = true;
  uEchoMessageObserver* observer;
  uEchoMessageHandler handler;
  void* obj;
  uEchoUdpServer* udpServer;
  uEchoMcastServer* mcastServer;

  if (!server)
    return false;

  uecho_server_stop(server);

  allActionsSucceeded &= uecho_mcast_serverlist_open(server->mcastServers);
  if (uecho_server_isudpserverenabled(server)) {
    allActionsSucceeded &= uecho_udp_serverlist_open(server->udpServers);
  }

  // Add message observers
  for (observer = uecho_message_observer_manager_getobservers(server->msgMgr); observer; observer = uecho_message_observer_next(observer)) {
    handler = uecho_message_observer_gethandler(observer);
    obj = uecho_message_observer_getobjcet(observer);
    if (!handler || !obj) {
      allActionsSucceeded = false;
      continue;
    }
    for (udpServer = uecho_udp_serverlist_gets(server->udpServers); udpServer; udpServer = uecho_udp_server_next(udpServer)) {
      allActionsSucceeded &= uecho_udp_server_addobserver(udpServer, obj, handler);
    }
    for (mcastServer = uecho_mcast_serverlist_gets(server->mcastServers); mcastServer; mcastServer = uecho_mcast_server_next(mcastServer)) {
      allActionsSucceeded &= uecho_mcast_server_addobserver(mcastServer, obj, handler);
    }
  }

  allActionsSucceeded &= uecho_mcast_serverlist_start(server->mcastServers);
  if (uecho_server_isudpserverenabled(server)) {
    allActionsSucceeded &= uecho_udp_serverlist_start(server->udpServers);
  }

  if (!allActionsSucceeded) {
    uecho_server_stop(server);
  }

  return allActionsSucceeded;
}

/****************************************
 * uecho_server_stop
 ****************************************/

bool uecho_server_stop(uEchoServer* server)
{
  bool allActionsSucceeded;

  if (!server)
    return false;

  allActionsSucceeded = true;

  allActionsSucceeded &= uecho_mcast_serverlist_close(server->mcastServers);
  allActionsSucceeded &= uecho_mcast_serverlist_stop(server->mcastServers);
  uecho_udp_serverlist_clear(server->mcastServers);

  allActionsSucceeded &= uecho_udp_serverlist_close(server->udpServers);
  allActionsSucceeded &= uecho_udp_serverlist_stop(server->udpServers);
  uecho_udp_serverlist_clear(server->udpServers);

  return allActionsSucceeded;
}

/****************************************
 * uecho_server_isrunning
 ****************************************/

bool uecho_server_isrunning(uEchoServer* server)
{
  bool allActionsSucceeded;

  if (!server)
    return false;

  allActionsSucceeded = true;
  allActionsSucceeded &= uecho_mcast_serverlist_isrunning(server->mcastServers);

  if (uecho_server_isudpserverenabled(server)) {
    allActionsSucceeded &= uecho_udp_serverlist_isrunning(server->udpServers);
  }

  return allActionsSucceeded;
}

/****************************************
 * uecho_server_postannounce
 ****************************************/

const char* uecho_server_getaddress(uEchoServer* server)
{
  uEchoMcastServer* mcastServer;

  for (mcastServer = uecho_mcast_serverlist_gets(server->mcastServers); mcastServer; mcastServer = uecho_mcast_server_next(mcastServer)) {
    return uecho_mcast_server_getaddress(mcastServer);
  }

  return "";
}

/****************************************
 * uecho_server_sendto
 ****************************************/

bool uecho_server_sendto(uEchoServer* server, const char* addr, int port, byte* msg, size_t msgLen)
{
  uEchoSocket* sock;
  size_t sentByteCnt;

  sock = uecho_socket_dgram_new();
  if (!sock)
    return false;

  sentByteCnt = uecho_socket_sendto(sock, addr, port, msg, msgLen);
  uecho_socket_delete(sock);
  return (msgLen == sentByteCnt) ? true : false;
}

/****************************************
 * uecho_server_postannounce
 ****************************************/

bool uecho_server_postannounce(uEchoServer* server, byte* msg, size_t msgLen)
{
  if (!server)
    return false;

  if (uecho_mcast_serverlist_post(server->mcastServers, msg, msgLen))
    return true;

  return uecho_server_sendto(server, uEchoMulticastAddr, uEchoUdpPort, msg, msgLen);
}

/****************************************
 * uecho_server_postresponse
 ****************************************/

bool uecho_server_postresponse(uEchoServer* server, const char* addr, byte* msg, size_t msgLen)
{
  if (!server)
    return false;

  if (uecho_udp_serverlist_sendto(server->udpServers, addr, msg, msgLen))
    return true;

  return uecho_server_sendto(server, addr, uEchoUdpPort, msg, msgLen);
}

/****************************************
 * uecho_server_addobserver
 ****************************************/

bool uecho_server_addobserver(uEchoServer* server, void* obj, uEchoMessageHandler handler)
{
  return uecho_message_observer_manager_addobserver(server->msgMgr, obj, handler);
}
