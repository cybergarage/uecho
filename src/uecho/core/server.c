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

uEchoServer* uecho_server_new(void)
{
  uEchoServer* server;

  server = (uEchoServer*)malloc(sizeof(uEchoServer));

  if (!server)
    return NULL;

  server->udp_servers = uecho_udp_serverlist_new();
  server->mcast_servers = uecho_mcast_serverlist_new();

  uecho_server_setoption(server, uEchoOptionNone);

  return server;
}

/****************************************
* uecho_server_delete
****************************************/

bool uecho_server_delete(uEchoServer* server)
{
  if (!server)
    return false;

  uecho_udp_serverlist_delete(server->udp_servers);
  uecho_mcast_serverlist_delete(server->mcast_servers);

  free(server);

  return true;
}

/****************************************
 * uecho_server_setmessagelistener
 ****************************************/

void uecho_server_setmessagelistener(uEchoServer* server, uEchoServerMessageListener listener)
{
  server->msg_listener = listener;
}

/****************************************
 * uecho_server_setuserdata
 ****************************************/

void uecho_server_setuserdata(uEchoServer* server, void* data)
{
  server->user_data = data;
}

/****************************************
 * uecho_server_getuserdata
 ****************************************/

void* uecho_server_getuserdata(uEchoServer* server)
{
  return server->user_data;
}

/****************************************
 * uecho_server_isboundaddress
 ****************************************/

bool uecho_server_isboundaddress(uEchoServer* server, const char* addr)
{
  if (!server)
    return false;

  if (uecho_udp_serverlist_isboundaddress(server->udp_servers, addr))
    return true;

  if (uecho_mcast_serverlist_isboundaddress(server->mcast_servers, addr))
    return true;

  return false;
}

/****************************************
 * uecho_server_start
 ****************************************/

bool uecho_server_start(uEchoServer* server)
{
  bool all_actions_succeeded = true;

  if (!server)
    return false;

  uecho_server_stop(server);

  all_actions_succeeded &= uecho_mcast_serverlist_open(server->mcast_servers);
  uecho_mcast_serverlist_setuserdata(server->mcast_servers, server);
  uecho_mcast_serverlist_setmessagelistener(server->mcast_servers, uecho_mcast_server_msglistener);
  all_actions_succeeded &= uecho_mcast_serverlist_start(server->mcast_servers);

  if (uecho_server_isudpserverenabled(server)) {
    all_actions_succeeded &= uecho_udp_serverlist_open(server->udp_servers);
    uecho_udp_serverlist_setuserdata(server->udp_servers, server);
    uecho_udp_serverlist_setmessagelistener(server->udp_servers, uecho_udp_server_msglistener);
    all_actions_succeeded &= uecho_udp_serverlist_start(server->udp_servers);
  }

  if (!all_actions_succeeded) {
    uecho_server_stop(server);
  }

  return all_actions_succeeded;
}

/****************************************
 * uecho_server_stop
 ****************************************/

bool uecho_server_stop(uEchoServer* server)
{
  bool all_actions_succeeded;

  if (!server)
    return false;

  all_actions_succeeded = true;

  all_actions_succeeded &= uecho_mcast_serverlist_close(server->mcast_servers);
  all_actions_succeeded &= uecho_mcast_serverlist_stop(server->mcast_servers);
  uecho_udp_serverlist_clear(server->mcast_servers);

  all_actions_succeeded &= uecho_udp_serverlist_close(server->udp_servers);
  all_actions_succeeded &= uecho_udp_serverlist_stop(server->udp_servers);
  uecho_udp_serverlist_clear(server->udp_servers);

  return all_actions_succeeded;
}

/****************************************
 * uecho_server_isrunning
 ****************************************/

bool uecho_server_isrunning(uEchoServer* server)
{
  bool all_actions_succeeded;

  if (!server)
    return false;

  all_actions_succeeded = true;
  all_actions_succeeded &= uecho_mcast_serverlist_isrunning(server->mcast_servers);

  if (uecho_server_isudpserverenabled(server)) {
    all_actions_succeeded &= uecho_udp_serverlist_isrunning(server->udp_servers);
  }

  return all_actions_succeeded;
}

/****************************************
 * uecho_server_performlistener
 ****************************************/

bool uecho_server_performlistener(uEchoServer* server, uEchoMessage* msg)
{
  if (!server)
    return false;

  if (!server->msg_listener)
    return false;

  server->msg_listener(server, msg);

  return true;
}

/****************************************
 * uecho_udp_server_msglistener
 ****************************************/

void uecho_udp_server_msglistener(uEchoUdpServer* udp_server, uEchoMessage* msg)
{
  uEchoServer* server = (uEchoServer*)uecho_udp_server_getuserdata(udp_server);

  if (!server)
    return;

  uecho_server_performlistener(server, msg);
}

/****************************************
 * uecho_mcast_server_msglistener
 ****************************************/

void uecho_mcast_server_msglistener(uEchoMcastServer* mcast_server, uEchoMessage* msg)
{
  uEchoServer* server = (uEchoServer*)uecho_mcast_server_getuserdata(mcast_server);

  if (!server)
    return;

  uecho_server_performlistener(server, msg);
}

/****************************************
 * uecho_server_postannounce
 ****************************************/

const char * uecho_server_getaddress(uEchoServer* server)
{
  return "";
}

/****************************************
 * uecho_server_postannounce
 ****************************************/

bool uecho_server_postannounce(uEchoServer* server, const byte* msg, size_t msg_len)
{
  return uecho_mcast_serverlist_post(server->mcast_servers, msg, msg_len);
}

/****************************************
 * uecho_server_postresponse
 ****************************************/

bool uecho_server_postresponse(uEchoServer* server, const char* addr, byte* msg, size_t msg_len)
{
  uEchoSocket* sock;
  size_t sent_byte_cnt;

  if (!server)
    return false;

  sock = uecho_socket_dgram_new();
  if (!sock)
    return false;

  // uecho_net_packet_debug(uecho_server_get const char* from_addr, const char* to_addr, const byte* msg_bytes, size_t msg_len);

  sent_byte_cnt = uecho_socket_sendto(sock, addr, uEchoUdpPort, msg, msg_len);

  uecho_socket_delete(sock);

  return (msg_len == sent_byte_cnt) ? true : false;
}

