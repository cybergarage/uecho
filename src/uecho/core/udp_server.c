/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/server.h>

/****************************************
* uecho_udp_server_new
****************************************/

uEchoUdpServer *uecho_udp_server_new()
{
	uEchoUdpServer *server;

	server = (uEchoUdpServer *)malloc(sizeof(uEchoUdpServer));

  if (!server)
    return NULL;
	
  server->socket = uecho_socket_dgram_new();
    
	return server;
}

/****************************************
 * uecho_udp_server_delete
 ****************************************/

void uecho_udp_server_delete(uEchoUdpServer *server)
{
  uecho_socket_delete(server->socket);
  
  free(server);
}

/****************************************
 * uecho_udp_server_performlistener
 ****************************************/

bool uecho_udp_server_performlistener(uEchoUdpServer *server, uEchoMessage *msg) {
  if (!server->msgListener)
    return false;
  server->msgListener(server, msg);
  return true;
}

/****************************************
 * uecho_udp_server_action
 ****************************************/

static void uecho_udp_server_action(uEchoThread *thread)
{
  uEchoUdpServer *server;
  uEchoDatagramPacket *dgmPkt;
  ssize_t dgmPktLen;
  uEchoMessage *msg;
  
  server = (uEchoUdpServer *)uecho_thread_getuserdata(thread);
  
  if (!uecho_socket_isbound(server->socket))
    return;
  
  while (uecho_thread_isrunnable(thread)) {
    dgmPkt = uecho_socket_datagram_packet_new();
    if (!dgmPkt)
      break;
    
    dgmPktLen = uecho_socket_recv(server->socket, dgmPkt);
    if (dgmPktLen < 0)
      break;
    
    msg = uecho_message_new();
    if (!msg)
      continue;
    
    if (uecho_message_parsepacket(msg, dgmPkt)) {
      uecho_udp_server_performlistener(server, msg);
    }
    
    uecho_message_delete(msg);
  }
}

/****************************************
 * uecho_udp_server_start
 ****************************************/

bool uecho_udp_server_start(uEchoUdpServer *server)
{
  uecho_udp_server_stop(server);
  
  // open udp socket
  
  server->socket = uecho_socket_dgram_new();
  if (!uecho_socket_bind(server->socket, uEchoUdpPort, "", false, true)) {
    uecho_udp_server_stop(server);
    return false;
  }
  
  // start server
  
  server->thread = uecho_thread_new();
  uecho_thread_setaction(server->thread, uecho_udp_server_action);
  uecho_thread_setuserdata(server->thread, server);
  if (!uecho_thread_start(server->thread)) {
    uecho_udp_server_stop(server);
    return false;
  }
  
  return true;
}

/****************************************
 * uecho_udp_server_stop
 ****************************************/

bool uecho_udp_server_stop(uEchoUdpServer *server)
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

/****************************************
 * uecho_udp_server_isrunning
 ****************************************/

bool uecho_udp_server_isrunning(uEchoUdpServer *server)
{
  if (!server->thread)
    return false;

  return uecho_thread_isrunning(server->thread);
}
