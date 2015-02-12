/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/net/socket.h>

#if defined(CG_NET_USE_SOCKET_LIST)

/****************************************
* uecho_socketlist_new
****************************************/

uEchoSocketList *uecho_socketlist_new()
{
	uecho_log_debug_l4("Entering...\n");

	uEchoSocketList *socketList = (uEchoSocketList *)malloc(sizeof(uEchoSocketList));

	if ( NULL != socketList )
		uecho_list_header_init((uEchoList *)socketList);

	return socketList;

	uecho_log_debug_l4("Leaving...\n");
}

/****************************************
* uecho_socketlist_delete
****************************************/

void uecho_socketlist_delete(uEchoSocketList *socketList)
{
	uecho_log_debug_l4("Entering...\n");

	uecho_socketlist_clear(socketList);
	free(socketList);

	uecho_log_debug_l4("Leaving...\n");
}

#endif
