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
#include <uecho/net/interface.h>

/****************************************
 * uecho_udp_serverlist_new
 ****************************************/

uEchoUdpServerList* uecho_udp_serverlist_new(void)
{
  uEchoUdpServerList* servers;

  servers = (uEchoUdpServerList*)malloc(sizeof(uEchoUdpServerList));
  if (!servers)
    return NULL;

  uecho_list_header_init((uEchoList*)servers);

  return servers;
}

/****************************************
 * uecho_udp_serverlist_delete
 ****************************************/

void uecho_udp_serverlist_delete(uEchoUdpServerList* servers)
{
  uecho_udp_serverlist_close(servers);
  uecho_udp_serverlist_stop(servers);
  uecho_udp_serverlist_clear(servers);

  free(servers);
}

/****************************************
 * uecho_udp_serverlist_setmessagelistener
 ****************************************/

void uecho_udp_serverlist_setmessagelistener(uEchoUdpServerList* servers, uEchoUdpServerMessageListener listener)
{
  uEchoUdpServer* server;

  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    uecho_udp_server_setmessagelistener(server, listener);
  }
}

/****************************************
 * uecho_udp_serverlist_setuserdata
 ****************************************/

void uecho_udp_serverlist_setuserdata(uEchoUdpServerList* servers, void* data)
{
  uEchoUdpServer* server;

  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    uecho_udp_server_setuserdata(server, data);
  }
}

/****************************************
 * uecho_udp_serverlist_open
 ****************************************/

bool uecho_udp_serverlist_open(uEchoUdpServerList* servers)
{
  uEchoUdpServer* server;
  uEchoNetworkInterfaceList* net_if_list;
  uEchoNetworkInterface* net_if;
  bool all_actions_succeeded;

  uecho_udp_serverlist_close(servers);

  net_if_list = uecho_net_interfacelist_new();

  if (uecho_net_gethostinterfaces(net_if_list) <= 0) {
    uecho_net_interfacelist_delete(net_if_list);
    return false;
  }

  all_actions_succeeded = true;

  for (net_if = uecho_net_interfacelist_gets(net_if_list); net_if; net_if = uecho_net_interface_next(net_if)) {
    server = uecho_udp_server_new();
    if (!server) {
      all_actions_succeeded = false;
      break;
    }

    if (!uecho_udp_server_open(server, uecho_net_interface_getaddress(net_if))) {
      all_actions_succeeded = false;
      uecho_udp_server_delete(server);
      break;
    }

    uecho_udp_serverlist_add(servers, server);
  }

  uecho_net_interfacelist_delete(net_if_list);

  if (!all_actions_succeeded) {
    uecho_udp_serverlist_close(servers);
    return false;
  }

  return true;
}

/****************************************
 * uecho_udp_serverlist_close
 ****************************************/

bool uecho_udp_serverlist_close(uEchoUdpServerList* servers)
{
  uEchoUdpServer* server;
  bool all_actions_succeeded;

  all_actions_succeeded = true;
  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    all_actions_succeeded &= uecho_udp_server_close(server);
  }

  return all_actions_succeeded;
}

/****************************************
 * uecho_udp_serverlist_start
 ****************************************/

bool uecho_udp_serverlist_start(uEchoUdpServerList* servers)
{
  uEchoUdpServer* server;
  bool all_actions_succeeded;

  uecho_udp_serverlist_stop(servers);

  all_actions_succeeded = true;
  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    all_actions_succeeded &= uecho_udp_server_start(server);
  }

  return all_actions_succeeded;
}

/****************************************
 * uecho_udp_serverlist_stop
 ****************************************/

bool uecho_udp_serverlist_stop(uEchoUdpServerList* servers)
{
  uEchoUdpServer* server;
  bool all_actions_succeeded;

  all_actions_succeeded = true;
  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    all_actions_succeeded &= uecho_udp_server_stop(server);
  }

  return all_actions_succeeded;
}

/****************************************
 * uecho_udp_serverlist_isrunning
 ****************************************/

bool uecho_udp_serverlist_isrunning(uEchoUdpServerList* servers)
{
  return (0 < uecho_udp_serverlist_size(servers)) ? true : false;
}

/****************************************
 * uecho_udp_serverlist_isboundaddress
 ****************************************/

bool uecho_udp_serverlist_isboundaddress(uEchoUdpServerList* servers, const char* addr)
{
  uEchoUdpServer* server;
  uEchoSocket* sock;

  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    sock = uecho_udp_getsocket(server);
    if (!sock)
      continue;
    if (uecho_socket_isboundaddress(sock, addr))
      return true;
  }

  return false;
}
