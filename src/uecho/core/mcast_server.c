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

uEchoMcastServer *uecho_mcast_server_new(void)
{
	uEchoMcastServer *server;

	server = (uEchoMcastServer *)malloc(sizeof(uEchoMcastServer));

  if (!server)
    return NULL;
	
  server->socket = NULL;
  server->thread = NULL;
  
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
 * uecho_mcast_server_open
 ****************************************/

bool uecho_mcast_server_open(uEchoMcastServer *server, const char *bindAddr)
{
  uecho_mcast_server_close(server);
  
  server->socket = uecho_socket_dgram_new();
  if (!uecho_socket_bind(server->socket, uEchoUdpPort, bindAddr, false, true)) {
    uecho_mcast_server_close(server);
    return false;
  }
  
  if (!uecho_socket_joingroup(server->socket, uEchoMulticastAddr, bindAddr)) {
    uecho_mcast_server_close(server);
    return false;
  }
  
  return true;
}

/****************************************
 * uecho_mcast_server_close
 ****************************************/

bool uecho_mcast_server_close(uEchoMcastServer *server)
{
  if (!server->socket)
    return true;
  
  uecho_socket_close(server->socket);
  uecho_socket_delete(server->socket);
  server->socket = NULL;
  
  return true;
}

/****************************************
 * uecho_mcast_server_isopened
 ****************************************/

bool uecho_mcast_server_isopened(uEchoMcastServer *server)
{
  if (!server->socket)
    return false;
  return true;
}

/****************************************
 * uecho_mcast_server_performlistener
 ****************************************/

bool uecho_mcast_server_performlistener(uEchoMcastServer *server, uEchoMessage *msg)
{
  if (!server->msgListener)
    return false;
  
  server->msgListener(server, msg);

  return true;
}

/****************************************
 * uecho_mcast_server_action
 ****************************************/

static void uecho_mcast_server_action(uEchoThread *thread)
{
  uEchoMcastServer *server;
  uEchoDatagramPacket *dgmPkt;
  ssize_t dgmPktLen;
  uEchoMessage *msg;
  
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
    
    msg = uecho_message_new();
    if (!msg)
      continue;
    
    if (uecho_message_parsepacket(msg, dgmPkt)) {
      uecho_mcast_server_performlistener(server, msg);
    }
    
    uecho_message_delete(msg);
  }
}

/****************************************
 * uecho_mcast_server_start
 ****************************************/

bool uecho_mcast_server_start(uEchoMcastServer *server)
{
  uecho_mcast_server_stop(server);

  if (!uecho_mcast_server_isopened(server))
    return false;
  
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
  if (!server->thread)
    return true;
  
  uecho_thread_stop(server->thread);
  uecho_thread_delete(server->thread);
  server->thread = NULL;
  
  return true;
}

/****************************************
 * uecho_mcast_server_isrunning
 ****************************************/

bool uecho_mcast_server_isrunning(uEchoMcastServer *server)
{
  if (!server->thread)
    return false;

  return uecho_thread_isrunning(server->thread);
}

/****************************************
 * uecho_mcast_server_post
 ****************************************/

bool uecho_mcast_server_post(uEchoMcastServer *server, byte *msg, size_t msgLen)
{
  size_t sentLen = 0;
  
  if (!server->socket)
    return false;
  
  sentLen = uecho_socket_sendto(server->socket, uEchoMulticastAddr, uEchoUdpPort, msg, msgLen);
  
  return (sentLen == msgLen) ? true : false;
}
