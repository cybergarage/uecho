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
#include <uecho/net/interface.h>

BOOST_AUTO_TEST_CASE(ServerTest)
{
  uEchoServer* server = uecho_server_new();

  BOOST_CHECK(server);
  BOOST_CHECK(uecho_server_start(server));
  BOOST_CHECK(uecho_server_isrunning(server));
  BOOST_CHECK(uecho_server_stop(server));

  uecho_server_delete(server);
}

BOOST_AUTO_TEST_CASE(UdpServerTest)
{
  uEchoNetworkInterfaceList* net_if_list = uecho_net_interfacelist_new();

  BOOST_CHECK(0 < uecho_net_gethostinterfaces(net_if_list));

  if (uecho_net_gethostinterfaces(net_if_list) <= 0) {
    uecho_net_interfacelist_delete(net_if_list);
    return;
  }

  uEchoNetworkInterface* net_if = uecho_net_interfacelist_gets(net_if_list);
  const char* bind_addr = uecho_net_interface_getaddress(net_if);

  uEchoUdpServer* server = uecho_udp_server_new();
  BOOST_CHECK(server);

  BOOST_CHECK(uecho_udp_server_open(server, bind_addr));
  BOOST_CHECK(uecho_udp_server_start(server));
  BOOST_CHECK(uecho_udp_server_stop(server));

  BOOST_CHECK(uecho_udp_server_delete(server));

  uecho_net_interfacelist_delete(net_if_list);
}

BOOST_AUTO_TEST_CASE(McastServerTest)
{
  uEchoNetworkInterfaceList* net_if_list = uecho_net_interfacelist_new();

  BOOST_CHECK(0 < uecho_net_gethostinterfaces(net_if_list));

  if (uecho_net_gethostinterfaces(net_if_list) <= 0) {
    uecho_net_interfacelist_delete(net_if_list);
    return;
  }

  uEchoNetworkInterface* net_if = uecho_net_interfacelist_gets(net_if_list);
  const char* bind_addr = uecho_net_interface_getaddress(net_if);

  uEchoMcastServer* server = uecho_mcast_server_new();
  BOOST_CHECK(server);

  BOOST_CHECK(uecho_mcast_server_open(server, bind_addr));
  BOOST_CHECK(uecho_mcast_server_start(server));
  BOOST_CHECK(uecho_mcast_server_stop(server));

  BOOST_CHECK(uecho_mcast_server_delete(server));

  uecho_net_interfacelist_delete(net_if_list);
}

BOOST_AUTO_TEST_CASE(UdpServerListTest)
{
  uEchoUdpServerList* servers = uecho_udp_serverlist_new();

  BOOST_CHECK(servers);
  BOOST_CHECK(uecho_udp_serverlist_open(servers));
  BOOST_CHECK(uecho_udp_serverlist_start(servers));
  BOOST_CHECK(uecho_udp_serverlist_isrunning(servers));
  BOOST_CHECK(uecho_udp_serverlist_stop(servers));

  uecho_udp_serverlist_delete(servers);
}

BOOST_AUTO_TEST_CASE(McastServerListTest)
{
  uEchoMcastServerList* servers = uecho_mcast_serverlist_new();

  BOOST_CHECK(servers);
  BOOST_CHECK(uecho_mcast_serverlist_open(servers));
  BOOST_CHECK(uecho_mcast_serverlist_start(servers));
  BOOST_CHECK(uecho_mcast_serverlist_isrunning(servers));
  BOOST_CHECK(uecho_mcast_serverlist_stop(servers));

  uecho_mcast_serverlist_delete(servers);
}
