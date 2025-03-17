/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/const.h>
#include <uecho/frame/server.h>
#include <uecho/util/logs.h>

/****************************************
 * uecho_mcast_server_new
 ****************************************/

uEchoMcastServer* uecho_mcast_server_new(void)
{
  uEchoMcastServer* server;

  server = (uEchoMcastServer*)malloc(sizeof(uEchoMcastServer));

  if (!server)
    return NULL;

  uecho_list_node_init((uEchoList*)server);

  server->socket = NULL;
  server->thread = NULL;
  server->msgMgr = uecho_message_observer_manager_new();

  return server;
}

/****************************************
 * uecho_mcast_server_delete
 ****************************************/

bool uecho_mcast_server_delete(uEchoMcastServer* server)
{
  if (!server)
    return false;

  uecho_mcast_server_stop(server);
  uecho_message_observer_manager_delete(server->msgMgr);
  uecho_mcast_server_remove(server);

  free(server);

  return true;
}

/****************************************
 * uecho_mcast_server_open
 ****************************************/

bool uecho_mcast_server_open(uEchoMcastServer* server, const char* bindAddr)
{
  uEchoSocketOption opt;

  if (!server)
    return false;

  uecho_mcast_server_close(server);

  server->socket = uecho_socket_dgram_new();

  uecho_socket_option_setbindinterface(&opt, false);
  uecho_socket_option_setreuseaddress(&opt, true);
  uecho_socket_option_setmulticastloop(&opt, true);

  if (!uecho_socket_bind(server->socket, uEchoUdpPort, bindAddr, &opt)) {
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

bool uecho_mcast_server_close(uEchoMcastServer* server)
{
  if (!server)
    return false;

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

bool uecho_mcast_server_isopened(uEchoMcastServer* server)
{
  if (!server)
    return false;

  if (!server->socket)
    return false;

  return true;
}

/****************************************
 * uecho_mcast_server_addobserver
 ****************************************/

bool uecho_mcast_server_addobserver(uEchoMcastServer* server, void* obj, uEchoMessageHandler handler)
{
  return uecho_message_observer_manager_addobserver(server->msgMgr, obj, handler);
}

/****************************************
 * uecho_mcast_server_action
 ****************************************/

static void uecho_mcast_server_action(uEchoThread* thread)
{
  uEchoMcastServer* server;
  uEchoDatagramPacket* dgmPkt;
  ssize_t dgmPktLen;
  uEchoMessage* msg;

  server = (uEchoMcastServer*)uecho_thread_getuserdata(thread);

  if (!server)
    return;

  if (!uecho_socket_isbound(server->socket))
    return;

  while (uecho_thread_isrunnable(thread)) {
    dgmPkt = uecho_socket_datagram_packet_new();
    if (!dgmPkt)
      break;

    dgmPktLen = uecho_socket_recv(server->socket, dgmPkt);
    if (dgmPktLen < 0) {
      uecho_socket_datagram_packet_delete(dgmPkt);
      break;
    }

    if (!uecho_thread_isrunnable(thread) || !uecho_socket_isbound(server->socket)) {
      uecho_socket_datagram_packet_delete(dgmPkt);
      break;
    }

    msg = uecho_message_new();
    if (!msg)
      continue;

    if (uecho_message_parsepacket(msg, dgmPkt)) {
      uecho_message_observer_manager_perform(server->msgMgr, msg);
    }
    else {
      uecho_net_datagram_packet_error(UECHO_LOG_NET_PREFIX_RECV, dgmPkt);
    }

    uecho_socket_datagram_packet_delete(dgmPkt);
    uecho_message_delete(msg);
  }
}

/****************************************
 * uecho_mcast_server_getaddress
 ****************************************/

const char* uecho_mcast_server_getaddress(uEchoMcastServer* server)
{
  if (!server->socket)
    return "";
  return uecho_socket_getaddress(server->socket);
}

/****************************************
 * uecho_mcast_server_start
 ****************************************/

bool uecho_mcast_server_start(uEchoMcastServer* server)
{
  if (!server)
    return false;

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

bool uecho_mcast_server_stop(uEchoMcastServer* server)
{
  if (!server)
    return false;

  if (!server->thread)
    return true;

  uecho_mcast_server_close(server);
  uecho_thread_stop(server->thread);
  uecho_thread_delete(server->thread);
  server->thread = NULL;

  return true;
}

/****************************************
 * uecho_mcast_server_isrunning
 ****************************************/

bool uecho_mcast_server_isrunning(uEchoMcastServer* server)
{
  if (!server)
    return false;

  if (!server->thread)
    return false;

  return uecho_thread_isrunning(server->thread);
}

/****************************************
 * uecho_mcast_server_post
 ****************************************/

bool uecho_mcast_server_post(uEchoMcastServer* server, const byte* msg, size_t msgLen)
{
  size_t sentLen = 0;

  if (!server)
    return false;

  if (!server->socket)
    return false;

  sentLen = uecho_socket_sendto(server->socket, uEchoMulticastAddr, uEchoUdpPort, msg, msgLen);

  return (sentLen == msgLen) ? true : false;
}
