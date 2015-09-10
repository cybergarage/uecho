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

#include <uecho/uecho.h>
#include <uecho/net/socket.h>
#include <uecho/net/interface.h>
#include <uecho/net/interface.h>

BOOST_AUTO_TEST_CASE(BindAddr)
{
  uEchoNetworkInterfaceList *netIfList = uecho_net_interfacelist_new();

  BOOST_CHECK(0 < uecho_net_gethostinterfaces(netIfList));
  
  if (uecho_net_gethostinterfaces(netIfList) <= 0) {
    uecho_net_interfacelist_delete(netIfList);
    return;
  }

  uEchoNetworkInterface *netIf = uecho_net_interfacelist_gets(netIfList);
  const char *bindAddr = uecho_net_interface_getaddress(netIf);
  
  uEchoSocket *sock = uecho_socket_dgram_new();

  // Unicast binding
  
  BOOST_CHECK(uecho_socket_bind(sock, uEchoUdpPort, bindAddr, true, true));
  BOOST_CHECK(uecho_socket_close(sock));
  
  // Multicast binding

  BOOST_CHECK(uecho_socket_bind(sock, uEchoUdpPort, bindAddr, false, true));
  BOOST_CHECK(uecho_socket_close(sock));
  
  uecho_socket_delete(sock);
  
  uecho_net_interfacelist_delete(netIfList);
}
