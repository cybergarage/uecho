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

uEchoMcastServerList *uecho_mcast_serverlist_new(void)
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
  uecho_mcast_serverlist_close(servers);
  uecho_mcast_serverlist_stop(servers);
	uecho_mcast_serverlist_clear(servers);

	free(servers);
}

/****************************************
 * uecho_mcast_serverlist_setmessagelistener
 ****************************************/

void uecho_mcast_serverlist_setmessagelistener(uEchoMcastServerList *servers, uEchoMcastServerMessageListener listener)
{
  uEchoMcastServer *server;

  for (server = uecho_mcast_serverlist_gets(servers); server; server = uecho_mcast_server_next(server))
{
     uecho_mcast_server_setmessagelistener(server, listener);
  }
}

/****************************************
 * uecho_mcast_serverlist_setuserdata
 ****************************************/

void uecho_mcast_serverlist_setuserdata(uEchoMcastServerList *servers, void *data)
{
  uEchoMcastServer *server;
  
  for (server = uecho_mcast_serverlist_gets(servers); server; server = uecho_mcast_server_next(server))
{
    uecho_mcast_server_setuserdata(server, data);
  }
}

/****************************************
 * uecho_mcast_serverlist_open
 ****************************************/

bool uecho_mcast_serverlist_open(uEchoMcastServerList *servers)
{
  uEchoMcastServer *server;
  uEchoNetworkInterfaceList *netIfList;
  uEchoNetworkInterface *netIf;
  bool allActionsSucceeded;

  uecho_mcast_serverlist_close(servers);
  
  netIfList = uecho_net_interfacelist_new();
  
  if (uecho_net_gethostinterfaces(netIfList) <= 0)
{
    uecho_net_interfacelist_delete(netIfList);
    return false;
  }
  
  allActionsSucceeded = true;

  for (netIf = uecho_net_interfacelist_gets(netIfList); netIf; netIf = uecho_net_interface_next(netIf))
{
    server = uecho_mcast_server_new();
    if (!server)
{
      allActionsSucceeded = false;
      break;
    }

    allActionsSucceeded &= uecho_mcast_server_open(server, uecho_net_interface_getaddress(netIf));

    uecho_mcast_serverlist_add(servers, server);
  }

  uecho_net_interfacelist_delete(netIfList);

  if (!allActionsSucceeded)
{
    uecho_mcast_serverlist_close(servers);
    return false;
  }
  
  return true;
}

/****************************************
 * uecho_mcast_serverlist_close
 ****************************************/

bool uecho_mcast_serverlist_close(uEchoMcastServerList *servers)
{
  uEchoMcastServer *server;
  bool allActionsSucceeded;

  allActionsSucceeded = true;
  for (server = uecho_mcast_serverlist_gets(servers); server; server = uecho_mcast_server_next(server))
{
    allActionsSucceeded &= uecho_mcast_server_close(server);
  }
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_mcast_serverlist_start
 ****************************************/

bool uecho_mcast_serverlist_start(uEchoMcastServerList *servers)
{
  uEchoMcastServer *server;
  bool allActionsSucceeded;
  
  uecho_mcast_serverlist_stop(servers);
  
  allActionsSucceeded = true;
  for (server = uecho_mcast_serverlist_gets(servers); server; server = uecho_mcast_server_next(server))
{
    allActionsSucceeded &= uecho_mcast_server_start(server);
  }
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_mcast_serverlist_stop
 ****************************************/

bool uecho_mcast_serverlist_stop(uEchoMcastServerList *servers)
{
  uEchoMcastServer *server;
  bool allActionsSucceeded;
  
  allActionsSucceeded = true;
  for (server = uecho_mcast_serverlist_gets(servers); server; server = uecho_mcast_server_next(server))
{
    allActionsSucceeded &= uecho_mcast_server_stop(server);
  }
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_mcast_serverlist_post
 ****************************************/

bool uecho_mcast_serverlist_post(uEchoMcastServerList *servers, byte *msg, size_t msgLen)
{
  uEchoMcastServer *server;

  bool allActionsSucceeded;
  
  allActionsSucceeded = true;
  for (server = uecho_mcast_serverlist_gets(servers); server; server = uecho_mcast_server_next(server))
{
    allActionsSucceeded &= uecho_mcast_server_post(server, msg, msgLen);
  }
  
  return allActionsSucceeded;
}

