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
* uecho_udp_server_new
****************************************/

uEchoUdpServer *uecho_udp_server_new(void)
{
	uEchoUdpServer *server;

	server = (uEchoUdpServer *)malloc(sizeof(uEchoUdpServer));

  if (!server)
    return NULL;
	
  server->socket = NULL;
  server->thread = NULL;
  
	return server;
}

/****************************************
 * uecho_udp_server_delete
 ****************************************/

void uecho_udp_server_delete(uEchoUdpServer *server)
{
  uecho_socket_delete(server->socket);
  uecho_udp_server_remove(server);
  
  free(server);
}

/****************************************
 * uecho_udp_server_open
 ****************************************/

bool uecho_udp_server_open(uEchoUdpServer *server, const char *bindAddr)
{
  uecho_udp_server_close(server);
  
  server->socket = uecho_socket_dgram_new();
  if (!uecho_socket_bind(server->socket, uEchoUdpPort, bindAddr, true, true)) {
    uecho_udp_server_close(server);
    return false;
  }
  
  return true;
}

/****************************************
 * uecho_udp_server_close
 ****************************************/

bool uecho_udp_server_close(uEchoUdpServer *server)
{
  if (!server->socket)
    return true;
  
  uecho_socket_close(server->socket);
  uecho_socket_delete(server->socket);
  server->socket = NULL;
  
  return true;
}

/****************************************
 * uecho_udp_server_isopened
 ****************************************/

bool uecho_udp_server_isopened(uEchoUdpServer *server)
{
  if (!server->socket)
    return false;
  return true;
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
  
  if (!uecho_udp_server_isopened(server))
    return false;
  
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
  if (!server->thread)
    return true;
  
  uecho_thread_stop(server->thread);
  uecho_thread_delete(server->thread);
  server->thread = NULL;
  
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
