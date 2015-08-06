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

/****************************************
* uecho_udp_server_new
****************************************/

uEchoUdpServer *uecho_udp_server_new()
{
	uEchoUdpServer *server;

	server = (uEchoUdpServer *)malloc(sizeof(uEchoUdpServer));

    if (!server)
        return NULL;
	
    server->socket = uecho_socket_dgram_new();
    
	return server;
}

/****************************************
 * uecho_udp_server_performlistener
 ****************************************/

bool uecho_udp_server_performlistener(uEchoUdpServer *server, uEchoMessage *msg) {
  if (!server->msgListener)
    return false;
  server->msgListener(server, msg);
  return true;
}

/****************************************
* uecho_udp_server_delete
****************************************/

void uecho_udp_server_delete(uEchoUdpServer *server)
{
    uecho_socket_delete(server->socket);
    
	free(server);
}

/****************************************
 * uecho_udp_server_start
 ****************************************/

bool uecho_udp_server_start(uEchoUdpServer *server)
{
    return true;
}

/****************************************
 * uecho_udp_server_stop
 ****************************************/

bool uecho_udp_server_stop(uEchoUdpServer *server)
{
    uecho_socket_close(server->socket);
    
    return true;
}
