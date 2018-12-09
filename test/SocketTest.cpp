/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
  uEchoNetworkInterfaceList* netIfList = uecho_net_interfacelist_new();

  BOOST_CHECK(0 < uecho_net_gethostinterfaces(netIfList));

  if (uecho_net_gethostinterfaces(netIfList) <= 0) {
    uecho_net_interfacelist_delete(netIfList);
    return;
  }

  uEchoNetworkInterface* netIf = uecho_net_interfacelist_gets(netIfList);
  const char* bindAddr = uecho_net_interface_getaddress(netIf);

  uEchoSocket* sock = uecho_socket_dgram_new();
  uEchoSocketOption* opt = uecho_socket_option_new();

  // Unicast binding

  uecho_socket_option_setbindinterface(opt, true);
  uecho_socket_option_setreuseaddress(opt, true);
  uecho_socket_option_setmulticastloop(opt, false);

  BOOST_CHECK(uecho_socket_bind(sock, uEchoUdpPort, bindAddr, opt));
  BOOST_CHECK(uecho_socket_close(sock));

  // Multicast binding

  uecho_socket_option_setbindinterface(opt, false);
  uecho_socket_option_setreuseaddress(opt, true);
  uecho_socket_option_setmulticastloop(opt, true);

  BOOST_CHECK(uecho_socket_bind(sock, uEchoUdpPort, bindAddr, opt));
  BOOST_CHECK(uecho_socket_close(sock));

  uecho_socket_option_delete(opt);
  uecho_socket_delete(sock);

  uecho_net_interfacelist_delete(netIfList);
}
