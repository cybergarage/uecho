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
#include <uecho/net/interface.h>

/****************************************
 * uecho_udp_serverlist_new
 ****************************************/

uEchoUdpServerList *uecho_udp_serverlist_new()
{
  uEchoUdpServerList *servers;
  
  servers = (uEchoUdpServerList *)malloc(sizeof(uEchoUdpServerList));
  if (!servers)
    return NULL;
  
  uecho_list_header_init((uEchoList *)servers);
  
  return servers;
}

/****************************************
 * uecho_udp_serverlist_delete
 ****************************************/

void uecho_udp_serverlist_delete(uEchoUdpServerList *servers)
{
  uecho_udp_serverlist_clear(servers);
  
  free(servers);
}

/****************************************
 * uecho_udp_serverlist_open
 ****************************************/

bool uecho_udp_serverlist_open(uEchoUdpServerList *servers)
{
  uEchoUdpServer *server;
  uEchoNetworkInterfaceList *netIfList;
  uEchoNetworkInterface *netIf;
  bool allActionsSucceeded;
  
  uecho_udp_serverlist_close(servers);
  
  if (uecho_net_gethostinterfaces(netIfList) <= 0) {
    uecho_net_interfacelist_delete(netIfList);
    return false;
  }
  
  allActionsSucceeded = true;
  
  for (netIf = uecho_net_interfacelist_gets(netIfList); netIf; netIf = uecho_net_interface_next(netIf)) {
    server = uecho_udp_server_new();
    if (!server) {
      allActionsSucceeded = false;
      break;
    }
    
    allActionsSucceeded &= uecho_udp_server_open(server, uecho_net_interface_getaddress(netIf));
    
    uecho_udp_serverlist_add(servers, server);
  }
  
  uecho_net_interfacelist_delete(netIfList);
  
  if (!allActionsSucceeded) {
    uecho_udp_serverlist_close(servers);
    return false;
  }
  
  return true;
}

/****************************************
 * uecho_udp_serverlist_close
 ****************************************/

bool uecho_udp_serverlist_close(uEchoUdpServerList *servers)
{
  uEchoUdpServer *server;
  bool allActionsSucceeded;
  
  allActionsSucceeded = true;
  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    allActionsSucceeded &= uecho_udp_server_close(server);
  }
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_udp_serverlist_start
 ****************************************/

bool uecho_udp_serverlist_start(uEchoUdpServerList *servers)
{
  uEchoUdpServer *server;
  bool allActionsSucceeded;
  
  uecho_udp_serverlist_stop(servers);
  
  allActionsSucceeded = true;
  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    allActionsSucceeded &= uecho_udp_server_start(server);
  }
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_udp_serverlist_stop
 ****************************************/

bool uecho_udp_serverlist_stop(uEchoUdpServerList *servers)
{
  uEchoUdpServer *server;
  bool allActionsSucceeded;
  
  allActionsSucceeded = true;
  for (server = uecho_udp_serverlist_gets(servers); server; server = uecho_udp_server_next(server)) {
    allActionsSucceeded &= uecho_udp_server_stop(server);
  }
  
  return allActionsSucceeded;
}
