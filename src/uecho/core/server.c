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
	
  server->udpServer = uecho_udp_server_new();
  uecho_udp_server_setuserdata(server->udpServer, server);
  uecho_udp_server_setmessagelistener(server->udpServer, uecho_udp_server_msglistener);
  
  server->mcastServer = uecho_mcast_server_new();
  uecho_mcast_server_setuserdata(server->mcastServer, server);
  uecho_mcast_server_setmessagelistener(server->mcastServer, uecho_mcast_server_msglistener);
  
	return server;
}

/****************************************
* uecho_server_delete
****************************************/

void uecho_server_delete(uEchoServer *server)
{
  uecho_udp_server_delete(server->udpServer);
  uecho_mcast_server_delete(server->mcastServer);
  
	free(server);
}

/****************************************
 * uecho_server_start
 ****************************************/

bool uecho_server_start(uEchoServer *server)
{
  bool allServerSuccess = true;
  
  allServerSuccess &= uecho_udp_server_start(server->udpServer);
  allServerSuccess &= uecho_mcast_server_start(server->mcastServer);
  
  return allServerSuccess;
}

/****************************************
 * uecho_server_stop
 ****************************************/

bool uecho_server_stop(uEchoServer *server)
{
  bool allServerSuccess = true;
  
  allServerSuccess &= uecho_udp_server_stop(server->udpServer);
  allServerSuccess &= uecho_mcast_server_stop(server->mcastServer);
  
  return allServerSuccess;
}

/****************************************
 * uecho_server_isrunning
 ****************************************/

bool uecho_server_isrunning(uEchoServer *server)
{
  bool allServerSuccess = true;
  
  allServerSuccess &= uecho_udp_server_isrunning(server->udpServer);
  allServerSuccess &= uecho_mcast_server_isrunning(server->mcastServer);
  
  return allServerSuccess;
  
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
