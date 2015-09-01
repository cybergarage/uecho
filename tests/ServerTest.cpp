/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>
#include <uecho/core/server.h>

BOOST_AUTO_TEST_CASE(ServerTest)
{
  uEchoServer *server = uecho_server_new();
  
  BOOST_CHECK(server);
  BOOST_CHECK(uecho_server_start(server));
  BOOST_CHECK(uecho_server_isrunning(server));
  BOOST_CHECK(uecho_server_stop(server));
  
  uecho_server_delete(server);
}

BOOST_AUTO_TEST_CASE(UdpServerTest)
{
  uEchoUdpServerList *servers = uecho_udp_serverlist_new();
  
  BOOST_CHECK(servers);
  BOOST_CHECK(uecho_udp_serverlist_open(servers));
  BOOST_CHECK(uecho_udp_serverlist_start(servers));
  BOOST_CHECK(uecho_udp_serverlist_isrunning(servers));
  BOOST_CHECK(uecho_udp_serverlist_stop(servers));
  
  uecho_udp_serverlist_delete(servers);
}

BOOST_AUTO_TEST_CASE(McastServerTest)
{
  uEchoMcastServerList *servers = uecho_mcast_serverlist_new();
  
  BOOST_CHECK(servers);
  BOOST_CHECK(uecho_mcast_serverlist_open(servers));
  BOOST_CHECK(uecho_mcast_serverlist_start(servers));
  BOOST_CHECK(uecho_mcast_serverlist_isrunning(servers));
  BOOST_CHECK(uecho_mcast_serverlist_stop(servers));
  
  uecho_mcast_serverlist_delete(servers);
}
