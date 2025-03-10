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

#include <uecho/net/interface.h>
#include <uecho/net/socket.h>
#include <uecho/uecho.h>

BOOST_AUTO_TEST_CASE(BindAddr)
{
  uEchoNetworkInterfaceList* net_if_list = uecho_net_interfacelist_new();

  BOOST_REQUIRE(0 < uecho_net_gethostinterfaces(net_if_list));

  if (uecho_net_gethostinterfaces(net_if_list) <= 0) {
    uecho_net_interfacelist_delete(net_if_list);
    return;
  }

  uEchoNetworkInterface* net_if = uecho_net_interfacelist_gets(net_if_list);
  const char* bind_addr = uecho_net_interface_getaddress(net_if);

  uEchoSocket* sock = uecho_socket_dgram_new();
  uEchoSocketOption* opt = uecho_socket_option_new();

  // Unicast binding

  uecho_socket_option_setbindinterface(opt, true);
  uecho_socket_option_setreuseaddress(opt, true);
  uecho_socket_option_setmulticastloop(opt, false);

  BOOST_REQUIRE(uecho_socket_bind(sock, uEchoUdpPort, bind_addr, opt));
  BOOST_REQUIRE(uecho_socket_close(sock));

  // Multicast binding

  uecho_socket_option_setbindinterface(opt, false);
  uecho_socket_option_setreuseaddress(opt, true);
  uecho_socket_option_setmulticastloop(opt, true);

  BOOST_REQUIRE(uecho_socket_bind(sock, uEchoUdpPort, bind_addr, opt));
  BOOST_REQUIRE(uecho_socket_close(sock));

  uecho_socket_option_delete(opt);
  uecho_socket_delete(sock);

  uecho_net_interfacelist_delete(net_if_list);
}
