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

#if defined(UECHO_NET_USE_SOCKET_LIST)

/****************************************
* uecho_socketlist_new
****************************************/

uEchoSocketList *uecho_socketlist_new()
{
	uEchoSocketList *socketList = (uEchoSocketList *)malloc(sizeof(uEchoSocketList));

	if ( NULL != socketList )
		uecho_list_header_init((uEchoList *)socketList);

	return socketList;
}

/****************************************
* uecho_socketlist_delete
****************************************/

void uecho_socketlist_delete(uEchoSocketList *socketList)
{
	uecho_socketlist_clear(socketList);
	free(socketList);
}

#endif
