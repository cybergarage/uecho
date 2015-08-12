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
#include <uecho/node.h>

BOOST_AUTO_TEST_CASE(Node)
{
  uEchoNode *node = uecho_node_new();
  BOOST_CHECK(node);
  
  uecho_node_delete(node);
}
