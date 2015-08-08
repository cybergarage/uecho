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

void uecho_udp_server_msglistener(uEchoUdpServer *server, uEchoMessage *msg);
void uecho_mcast_server_msglistener(uEchoMcastServer *server, uEchoMessage *msg);

/****************************************
* uecho_server_new
****************************************/

uEchoServer *uecho_server_new()
{
	uEchoServer *server;

	server = (uEchoServer *)malloc(sizeof(uEchoServer));

  if (!server)
  return NULL;
	
  server->udpServers = uecho_udp_serverlist_new();
  
  server->mcastServers = uecho_mcast_serverlist_new();
  
	return server;
}

/****************************************
* uecho_server_delete
****************************************/

void uecho_server_delete(uEchoServer *server)
{
  uecho_udp_serverlist_delete(server->udpServers);
  uecho_mcast_serverlist_delete(server->mcastServers);
  
	free(server);
}

/****************************************
 * uecho_server_start
 ****************************************/

bool uecho_server_start(uEchoServer *server)
{
  bool allActionsSucceeded = true;
  
  uecho_server_stop(server);
  
  allActionsSucceeded &= uecho_udp_serverlist_open(server->udpServers);
  allActionsSucceeded &= uecho_udp_serverlist_start(server->udpServers);
  uecho_udp_serverlist_setuserdata(server->udpServers, server);
  uecho_udp_serverlist_setmessagelistener(server->udpServers, uecho_udp_server_msglistener);
  
  allActionsSucceeded &= uecho_mcast_serverlist_open(server->mcastServers);
  allActionsSucceeded &= uecho_mcast_serverlist_start(server->mcastServers);
  uecho_mcast_serverlist_setuserdata(server->mcastServers, server);
  uecho_mcast_serverlist_setmessagelistener(server->mcastServers, uecho_mcast_server_msglistener);

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
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_udp_serverlist_close(server->udpServers);
  allActionsSucceeded &= uecho_udp_serverlist_stop(server->udpServers);
  uecho_udp_serverlist_clear(server->udpServers);
  
  allActionsSucceeded &= uecho_mcast_serverlist_close(server->mcastServers);
  allActionsSucceeded &= uecho_mcast_serverlist_stop(server->mcastServers);
  uecho_udp_serverlist_clear(server->mcastServers);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_server_isrunning
 ****************************************/

bool uecho_server_isrunning(uEchoServer *server)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= (0 < uecho_udp_serverlist_size(server->udpServers)) ? true : false;
  allActionsSucceeded &= (0 < uecho_udp_serverlist_size(server->mcastServers)) ? true : false;
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_server_performlistener
 ****************************************/

bool uecho_server_performlistener(uEchoServer *server, uEchoMessage *msg) {
  if (!server->msgListener)
    return false;
  server->msgListener(server, msg);
  return true;
}

/****************************************
 * uecho_udp_server_msglistener
 ****************************************/

void uecho_udp_server_msglistener(uEchoUdpServer *udpServer, uEchoMessage *msg) {
  uEchoServer *server = (uEchoServer *)uecho_udp_server_getuserdata(udpServer);
  if (!server)
    return;
  uecho_server_performlistener(server, msg);
}

/****************************************
 * uecho_mcast_server_msglistener
 ****************************************/

void uecho_mcast_server_msglistener(uEchoMcastServer *mcastServer, uEchoMessage *msg) {
  uEchoServer *server = (uEchoServer *)uecho_mcast_server_getuserdata(mcastServer);
  if (!server)
    return;
  uecho_server_performlistener(server, msg);
}
