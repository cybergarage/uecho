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
#include <uecho/node.h>

BOOST_AUTO_TEST_CASE(Node)
{
  uEchoNode *node = uecho_node_new();
  BOOST_CHECK(node);
  
  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeSetObjects)
{
  uEchoNode *node = uecho_node_new();
  BOOST_CHECK(node);
  
  const int uEchoTestObjectCodeMax = uEchoObjectCodeMax % 100;
  
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), 0);
  
  for (size_t n=uEchoObjectCodeMin; n<=uEchoTestObjectCodeMax; n++)
{
    BOOST_CHECK(uecho_node_setobject(node, (uEchoObjectCode)n));
  }
  
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));
  
  for (size_t n=uEchoObjectCodeMin; n<=uEchoTestObjectCodeMax; n++)
{
    uEchoObject *obj = uecho_node_getobjectbycode(node, (uEchoObjectCode)n);
    BOOST_CHECK(obj);
    BOOST_CHECK_EQUAL(uecho_object_getcode(obj), n);
  }
  
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));
  
  uecho_node_delete(node);
}
