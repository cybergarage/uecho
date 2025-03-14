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
  server->msg_mgr = uecho_message_observer_manager_new();

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
  uecho_message_observer_manager_delete(server->msg_mgr);

  free(server);

  return true;
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
  uEchoMessageObserver* observer;
  uEchoMessageHandler handler;
  void* obj;
  uEchoUdpServer* udp_server;
  uEchoMcastServer* mcast_server;

  if (!server)
    return false;

  uecho_server_stop(server);

  all_actions_succeeded &= uecho_mcast_serverlist_open(server->mcast_servers);
  if (uecho_server_isudpserverenabled(server)) {
    all_actions_succeeded &= uecho_udp_serverlist_open(server->udp_servers);
  }

  // Add message observers
  for (observer = uecho_message_observer_manager_getobservers(server->msg_mgr); observer; observer = uecho_message_observer_next(observer)) {
    handler = uecho_message_observer_gethandler(observer);
    obj = uecho_message_observer_getobjcet(observer);
    if (!handler || !obj) {
      all_actions_succeeded = false;
      continue;
    }
    for (udp_server = uecho_udp_serverlist_gets(server->udp_servers); udp_server; udp_server = uecho_udp_server_next(udp_server)) {
      all_actions_succeeded &= uecho_udp_server_addobserver(udp_server, obj, handler);
    }
    for (mcast_server = uecho_mcast_serverlist_gets(server->mcast_servers); mcast_server; mcast_server = uecho_mcast_server_next(mcast_server)) {
      all_actions_succeeded &= uecho_mcast_server_addobserver(mcast_server, obj, handler);
    }
  }

  all_actions_succeeded &= uecho_mcast_serverlist_start(server->mcast_servers);
  if (uecho_server_isudpserverenabled(server)) {
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
 * uecho_server_postannounce
 ****************************************/

const char* uecho_server_getaddress(uEchoServer* server)
{
  uEchoMcastServer* mcast_server;

  for (mcast_server = uecho_mcast_serverlist_gets(server->mcast_servers); mcast_server; mcast_server = uecho_mcast_server_next(mcast_server)) {
    return uecho_mcast_server_getaddress(mcast_server);
  }

  return "";
}

/****************************************
 * uecho_server_sendto
 ****************************************/

bool uecho_server_sendto(uEchoServer* server, const char* addr, int port, byte* msg, size_t msg_len)
{
  uEchoSocket* sock;
  size_t sent_byte_cnt;

  sock = uecho_socket_dgram_new();
  if (!sock)
    return false;

  sent_byte_cnt = uecho_socket_sendto(sock, addr, port, msg, msg_len);
  uecho_socket_delete(sock);
  return (msg_len == sent_byte_cnt) ? true : false;
}

/****************************************
 * uecho_server_postannounce
 ****************************************/

bool uecho_server_postannounce(uEchoServer* server, byte* msg, size_t msg_len)
{
  if (!server)
    return false;

  if (uecho_mcast_serverlist_post(server->mcast_servers, msg, msg_len))
    return true;

  return uecho_server_sendto(server, uEchoMulticastAddr, uEchoUdpPort, msg, msg_len);
}

/****************************************
 * uecho_server_postresponse
 ****************************************/

bool uecho_server_postresponse(uEchoServer* server, const char* addr, byte* msg, size_t msg_len)
{
  if (!server)
    return false;

  if (uecho_udp_serverlist_sendto(server->udp_servers, addr, msg, msg_len))
    return true;

  return uecho_server_sendto(server, addr, uEchoUdpPort, msg, msg_len);
}

/****************************************
 * uecho_server_addobserver
 ****************************************/

bool uecho_server_addobserver(uEchoServer* server, void* obj, uEchoMessageHandler handler)
{
  return uecho_message_observer_manager_addobserver(server->msg_mgr, obj, handler);
}
