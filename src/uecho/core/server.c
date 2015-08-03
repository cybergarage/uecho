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
* uecho_server_new
****************************************/

uEchoServer *uecho_server_new()
{
	uEchoServer *server;

	server = (uEchoServer *)malloc(sizeof(uEchoServer));

  if (!server)
  return NULL;
	
  server->udpServer = uecho_udp_server_new();
  server->mcastServer = uecho_mcast_server_new();
  
	return server;
}

/****************************************
* uecho_server_delete
****************************************/

void uecho_server_delete(uEchoServer *server)
{
  uecho_udp_server_delete(server->udpServer);
  uecho_mcast_server_delete(server->mcastServer);
  
	free(server);
}

/****************************************
 * uecho_server_start
 ****************************************/

bool uecho_server_start(uEchoServer *server)
{
  bool allServerSuccess = true;
  
  allServerSuccess &= uecho_udp_server_start(server->udpServer);
  allServerSuccess &= uecho_mcast_server_start(server->mcastServer);
  
  return allServerSuccess;
}

/****************************************
 * uecho_server_stop
 ****************************************/

bool uecho_server_stop(uEchoServer *server)
{
  bool allServerSuccess = true;
  
  allServerSuccess &= uecho_udp_server_stop(server->udpServer);
  allServerSuccess &= uecho_mcast_server_stop(server->mcastServer);
  
  return allServerSuccess;
}

/****************************************
 * uecho_server_isrunning
 ****************************************/

bool uecho_server_isrunning(uEchoServer *server)
{
  bool allServerSuccess = true;
  
  allServerSuccess &= uecho_udp_server_isrunning(server->udpServer);
  allServerSuccess &= uecho_mcast_server_isrunning(server->mcastServer);
  
  return allServerSuccess;
  
}
