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

#include <uecho/node_internal.h>

BOOST_AUTO_TEST_CASE(NodeListAdd)
{
  uEchoNodeList* nodes = uecho_nodelist_new();

  BOOST_CHECK_EQUAL(uecho_nodelist_size(nodes), 0);

  for (int n = 0; n < 10; n++) {
    BOOST_CHECK(uecho_nodelist_add(nodes, uecho_node_new()));
    BOOST_CHECK_EQUAL(uecho_nodelist_size(nodes), (n + 1));
  }

  uecho_nodelist_clear(nodes);
  BOOST_CHECK_EQUAL(uecho_nodelist_size(nodes), 0);

  uecho_nodelist_delete(nodes);
}
