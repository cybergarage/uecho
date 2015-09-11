/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/server.h>

/****************************************
* uecho_server_new
****************************************/

uEchoServer *uecho_server_new(void)
{
  uEchoServer *server;

  server = (uEchoServer *)malloc(sizeof(uEchoServer));

  if (!server)
    return NULL;
  
  server->udpServers = uecho_udp_serverlist_new();
  server->mcastServers = uecho_mcast_serverlist_new();
  
  uecho_server_setoption(server, uEchoOptionNone);
  
  return server;
}

/****************************************
* uecho_server_delete
****************************************/

bool uecho_server_delete(uEchoServer *server)
{
  if (!server)
    return false;
    
  uecho_udp_serverlist_delete(server->udpServers);
  uecho_mcast_serverlist_delete(server->mcastServers);
  
  free(server);
  
  return true;
}

/****************************************
 * uecho_server_setmessagelistener
 ****************************************/

void uecho_server_setmessagelistener(uEchoServer *server, uEchoServerMessageListener listener)
{
  server->msgListener = listener;
}
  
/****************************************
 * uecho_server_setuserdata
 ****************************************/

void uecho_server_setuserdata(uEchoServer *server, void *data)
{
  server->userData = data;
}
    
/****************************************
 * uecho_server_getuserdata
 ****************************************/

void *uecho_server_getuserdata(uEchoServer *server)
{
  return server->userData;
}

/****************************************
 * uecho_server_isboundaddress
 ****************************************/

bool uecho_server_isboundaddress(uEchoServer *server, const char *addr)
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

bool uecho_server_start(uEchoServer *server)
{
  bool allActionsSucceeded = true;
  
  if (!server)
    return false;

  uecho_server_stop(server);
  
  allActionsSucceeded &= uecho_mcast_serverlist_open(server->mcastServers);
  uecho_mcast_serverlist_setuserdata(server->mcastServers, server);
  uecho_mcast_serverlist_setmessagelistener(server->mcastServers, uecho_mcast_server_msglistener);
  allActionsSucceeded &= uecho_mcast_serverlist_start(server->mcastServers);

  if (uecho_server_isudpserverenabled(server)) {
    allActionsSucceeded &= uecho_udp_serverlist_open(server->udpServers);
    uecho_udp_serverlist_setuserdata(server->udpServers, server);
    uecho_udp_serverlist_setmessagelistener(server->udpServers, uecho_udp_server_msglistener);
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

bool uecho_server_stop(uEchoServer *server)
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

bool uecho_server_isrunning(uEchoServer *server)
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
 * uecho_server_performlistener
 ****************************************/

bool uecho_server_performlistener(uEchoServer *server, uEchoMessage *msg)
{
  if (!server)
    return false;

  if (!server->msgListener)
    return false;
    
  server->msgListener(server, msg);
  
  return true;
}

/****************************************
 * uecho_udp_server_msglistener
 ****************************************/

void uecho_udp_server_msglistener(uEchoUdpServer *udpServer, uEchoMessage *msg)
{
  uEchoServer *server = (uEchoServer *)uecho_udp_server_getuserdata(udpServer);

  if (!server)
    return;

  uecho_server_performlistener(server, msg);
}

/****************************************
 * uecho_mcast_server_msglistener
 ****************************************/

void uecho_mcast_server_msglistener(uEchoMcastServer *mcastServer, uEchoMessage *msg)
{
  uEchoServer *server = (uEchoServer *)uecho_mcast_server_getuserdata(mcastServer);

  if (!server)
    return;

  uecho_server_performlistener(server, msg);
}

/****************************************
 * uecho_server_postannounce
 ****************************************/

bool uecho_server_postannounce(uEchoServer *server, const byte *msg, size_t msgLen)
{
  return uecho_mcast_serverlist_post(server->mcastServers, msg, msgLen);
}

/****************************************
 * uecho_server_postresponse
 ****************************************/

bool uecho_server_postresponse(uEchoServer *server, const char *addr, byte *msg, size_t msgLen)
{
  uEchoSocket *sock;
  size_t sentByteCnt;
  
  if (!server)
    return false;
 
  sock = uecho_socket_dgram_new();
  if (!sock)
    return false;

  sentByteCnt = uecho_socket_sendto(sock, addr, uEchoUdpPort, msg, msgLen);

  uecho_socket_delete(sock);

  return (msgLen == sentByteCnt) ? true : false;
}
