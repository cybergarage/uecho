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
#include <uecho/util/logs.h>

/****************************************
 * uecho_udp_server_new
 ****************************************/

uEchoUdpServer* uecho_udp_server_new(void)
{
  uEchoUdpServer* server;

  server = (uEchoUdpServer*)malloc(sizeof(uEchoUdpServer));

  if (!server)
    return NULL;

  uecho_list_node_init((uEchoList*)server);

  server->socket = NULL;
  server->thread = NULL;
  server->msgMgr = uecho_message_observer_manager_new();

  return server;
}

/****************************************
 * uecho_udp_server_delete
 ****************************************/

bool uecho_udp_server_delete(uEchoUdpServer* server)
{
  if (!server)
    return false;

  uecho_socket_delete(server->socket);
  uecho_message_observer_manager_delete(server->msgMgr);
  uecho_udp_server_remove(server);

  free(server);

  return true;
}

/****************************************
 * uecho_udp_server_getaddress
 ****************************************/

const char* uecho_udp_server_getaddress(uEchoUdpServer* server)
{
  if (!server->socket)
    return "";
  return uecho_socket_getaddress(server->socket);
}

/****************************************
 * uecho_udp_server_open
 ****************************************/

bool uecho_udp_server_open(uEchoUdpServer* server, const char* bindAddr)
{
  uEchoSocketOption opt;

  if (!server)
    return false;

  uecho_udp_server_close(server);

  server->socket = uecho_socket_dgram_new();

  uecho_socket_option_setbindinterface(&opt, true);
  uecho_socket_option_setreuseaddress(&opt, true);
  uecho_socket_option_setmulticastloop(&opt, false);

  if (!uecho_socket_bind(server->socket, uEchoUdpPort, bindAddr, &opt)) {
    uecho_udp_server_close(server);
    return false;
  }

  return true;
}

/****************************************
 * uecho_udp_server_close
 ****************************************/

bool uecho_udp_server_close(uEchoUdpServer* server)
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
 * uecho_udp_server_isopened
 ****************************************/

bool uecho_udp_server_isopened(uEchoUdpServer* server)
{
  if (!server)
    return false;

  if (!server->socket)
    return false;

  return true;
}

/****************************************
 * uecho_udp_server_addobserver
 ****************************************/

bool uecho_udp_server_addobserver(uEchoUdpServer* server, void* obj, uEchoMessageHandler handler)
{
  return uecho_message_observer_manager_addobserver(server->msgMgr, obj, handler);
}

/****************************************
 * uecho_udp_server_action
 ****************************************/

static void uecho_udp_server_action(uEchoThread* thread)
{
  uEchoUdpServer* server;
  uEchoDatagramPacket* dgmPkt;
  ssize_t dgmPktLen;
  uEchoMessage* msg;

  server = (uEchoUdpServer*)uecho_thread_getuserdata(thread);

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
 * uecho_udp_server_start
 ****************************************/

bool uecho_udp_server_start(uEchoUdpServer* server)
{
  if (!server)
    return false;

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

bool uecho_udp_server_stop(uEchoUdpServer* server)
{
  if (!server)
    return false;

  if (!server->thread)
    return true;

  uecho_udp_server_close(server);
  uecho_thread_stop(server->thread);
  uecho_thread_delete(server->thread);
  server->thread = NULL;

  return true;
}

/****************************************
 * uecho_udp_server_isrunning
 ****************************************/

bool uecho_udp_server_isrunning(uEchoUdpServer* server)
{
  if (!server)
    return false;

  if (!server->thread)
    return false;

  return uecho_thread_isrunning(server->thread);
}
