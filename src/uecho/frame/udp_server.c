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
  uecho_udp_server_remove(server);

  free(server);

  return true;
}

/****************************************
 * uecho_udp_server_setmessagelistener
 ****************************************/

void uecho_udp_server_setmessagelistener(uEchoUdpServer* server, uEchoUdpServerMessageListener listener)
{
  server->msg_listener = listener;
}

/****************************************
 * uecho_udp_server_setuserdata
 ****************************************/

void uecho_udp_server_setuserdata(uEchoUdpServer* server, void* data)
{
  server->user_data = data;
}

/****************************************
 * uecho_udp_server_getuserdata
 ****************************************/

void* uecho_udp_server_getuserdata(uEchoUdpServer* server)
{
  return server->user_data;
}

/****************************************
 * uecho_udp_server_getaddress
 ****************************************/

const char * uecho_udp_server_getaddress(uEchoUdpServer* server)
{
  if (!server->socket)
    return "";
  return uecho_socket_getaddress(server->socket);
}

/****************************************
 * uecho_udp_server_open
 ****************************************/

bool uecho_udp_server_open(uEchoUdpServer* server, const char* bind_addr)
{
  uEchoSocketOption opt;

  if (!server)
    return false;

  uecho_udp_server_close(server);

  server->socket = uecho_socket_dgram_new();

  uecho_socket_option_setbindinterface(&opt, true);
  uecho_socket_option_setreuseaddress(&opt, true);
  uecho_socket_option_setmulticastloop(&opt, false);

  if (!uecho_socket_bind(server->socket, uEchoUdpPort, bind_addr, &opt)) {
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
 * uecho_udp_server_performlistener
 ****************************************/

bool uecho_udp_server_performlistener(uEchoUdpServer* server, uEchoMessage* msg)
{
  if (!server)
    return false;

  if (!server->msg_listener)
    return false;

  server->msg_listener(server, msg);

  return true;
}

/****************************************
 * uecho_udp_server_action
 ****************************************/

static void uecho_udp_server_action(uEchoThread* thread)
{
  uEchoUdpServer* server;
  uEchoDatagramPacket* dgm_pkt;
  ssize_t dgm_pkt_len;
  uEchoMessage* msg;

  server = (uEchoUdpServer*)uecho_thread_getuserdata(thread);

  if (!server)
    return;

  if (!uecho_socket_isbound(server->socket))
    return;

  while (uecho_thread_isrunnable(thread)) {
    dgm_pkt = uecho_socket_datagram_packet_new();
    if (!dgm_pkt)
      break;

    dgm_pkt_len = uecho_socket_recv(server->socket, dgm_pkt);
    if (dgm_pkt_len < 0)
      break;

    if (!uecho_thread_isrunnable(thread) || !uecho_socket_isbound(server->socket))
      break;

    msg = uecho_message_new();
    if (!msg)
      continue;

    if (uecho_message_parsepacket(msg, dgm_pkt)) {
      uecho_udp_server_performlistener(server, msg);
    } else {
      uecho_net_datagram_packet_error(UECHO_LOG_NET_PREFIX_RECV, dgm_pkt);
    }

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
