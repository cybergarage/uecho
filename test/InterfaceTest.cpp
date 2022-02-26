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

BOOST_AUTO_TEST_CASE(GetInterface)
{
  uEchoNetworkInterfaceList* net_if_list = uecho_net_interfacelist_new();

  BOOST_CHECK(0 < uecho_net_gethostinterfaces(net_if_list));

  uecho_net_interfacelist_delete(net_if_list);
}
