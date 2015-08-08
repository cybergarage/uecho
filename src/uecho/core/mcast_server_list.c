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
* uecho_mcast_serverlist_new
****************************************/

uEchoMcastServerList *uecho_mcast_serverlist_new()
{
	uEchoMcastServerList *servers;

	servers = (uEchoMcastServerList *)malloc(sizeof(uEchoMcastServerList));
  if (!servers)
    return NULL;

  uecho_list_header_init((uEchoList *)servers);

	return servers;
}

/****************************************
* uecho_mcast_serverlist_delete
****************************************/

void uecho_mcast_serverlist_delete(uEchoMcastServerList *servers)
{
	uecho_mcast_serverlist_clear(servers);

	free(servers);
}

/****************************************
 * uecho_mcast_serverlist_open
 ****************************************/

void uecho_mcast_serverlist_open(uEchoMcastServerList *servers)
{
  uEchoNetworkInterfaceList *netIfList;
  uEchoNetworkInterface *i;
/*
  if int uecho_net_gethostinterfaces(netIfList);
  
  for (i = uecho_net_interfacelist_gets(ifList); i; i = uecho_net_interface_next(i)) {
    
  }
  ifList = uecho_net_interfacelist_new();
*/

  uecho_net_interfacelist_delete(netIfList);
}
