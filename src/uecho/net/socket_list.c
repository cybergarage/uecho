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

mUpnpSocketList *uecho_socketlist_new()
{
	uecho_log_debug_l4("Entering...\n");

	mUpnpSocketList *socketList = (mUpnpSocketList *)malloc(sizeof(mUpnpSocketList));

	if ( NULL != socketList )
		uecho_list_header_init((mUpnpList *)socketList);

	return socketList;

	uecho_log_debug_l4("Leaving...\n");
}

/****************************************
* uecho_socketlist_delete
****************************************/

void uecho_socketlist_delete(mUpnpSocketList *socketList)
{
	uecho_log_debug_l4("Entering...\n");

	uecho_socketlist_clear(socketList);
	free(socketList);

	uecho_log_debug_l4("Leaving...\n");
}

#endif
