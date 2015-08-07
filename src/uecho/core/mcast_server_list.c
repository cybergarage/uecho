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
