/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/const.h>
#include <uecho/core/server.h>

/****************************************
* uecho_mcast_server_new
****************************************/

uEchoMcastServer *uecho_mcast_server_new()
{
	uEchoMcastServer *server;

	server = (uEchoMcastServer *)malloc(sizeof(uEchoMcastServer));

  if (!server)
    return NULL;
	
  server->socket = uecho_socket_dgram_new();
  
	return server;
}

/****************************************
* uecho_mcast_server_delete
****************************************/

void uecho_mcast_server_delete(uEchoMcastServer *server)
{
  uecho_mcast_server_stop(server);
  
	free(server);
}

/****************************************
 * uecho_mcast_server_action
 ****************************************/

static void uecho_mcast_server_action(uEchoThread *thread)
{
  uEchoMcastServer *server;
  uEchoDatagramPacket *dgmPkt;
  ssize_t dgmPktLen;
  
  server = (uEchoMcastServer *)uecho_thread_getuserdata(thread);

  if (!uecho_socket_isbound(server->socket))
    return;
  
  while (uecho_thread_isrunnable(thread)) {
    dgmPkt = uecho_socket_datagram_packet_new();
    if (!dgmPkt)
      break;
 
    dgmPktLen = uecho_socket_recv(server->socket, dgmPkt);
    if (dgmPktLen < 0)
      break;
  }
}

/****************************************
 * uecho_mcast_server_start
 ****************************************/

bool uecho_mcast_server_start(uEchoMcastServer *server)
{
  uecho_mcast_server_stop(server);

  // open multicast socket
  
  server->socket = uecho_socket_dgram_new();
  if (!uecho_socket_bind(server->socket, uEchoUdpPort, "", false, true)) {
    uecho_mcast_server_stop(server);
    return false;
  }
  
  if (!uecho_socket_joingroup(server->socket, uEchoMulticastAddr, "")) {
    uecho_mcast_server_stop(server);
    return false;
  }

  // start server
  
  server->thread = uecho_thread_new();
  uecho_thread_setaction(server->thread, uecho_mcast_server_action);
  uecho_thread_setuserdata(server->thread, server);
  if (!uecho_thread_start(server->thread)) {
    uecho_mcast_server_stop(server);
    return false;
  }
  
  return true;
}

/****************************************
 * uecho_mcast_server_stop
 ****************************************/

bool uecho_mcast_server_stop(uEchoMcastServer *server)
{
  // close multicast socket
  
  if (server->socket) {
    uecho_socket_close(server->socket);
    uecho_socket_delete(server->socket);
    server->socket = NULL;
  }
  
  // stop server
  
  if (server->thread) {
    uecho_thread_stop(server->thread);
    uecho_thread_delete(server->thread);
    server->thread = NULL;
  }
  
  return true;
}
