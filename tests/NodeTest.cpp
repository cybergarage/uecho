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
#include <uecho/profile.h>

BOOST_AUTO_TEST_CASE(NodeDefault)
{
  uEchoNode *node = uecho_node_new();
  BOOST_CHECK(node);
  
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), 1);
  BOOST_CHECK(uecho_node_hasobjectbycode(node, uEchoNodeProfileObject));
  
  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeSetObjects)
{
  uEchoNode *node = uecho_node_new();
  BOOST_CHECK(node);
  
  const int uEchoTestObjectCodeMax = uEchoObjectCodeMax % 100;

  uecho_node_clear(node);
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), 0);
  
  for (size_t n=uEchoObjectCodeMin; n<=uEchoTestObjectCodeMax; n++) {
    uecho_node_setobject(node, (uEchoObjectCode)n);
  }
  
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));
  
  for (size_t n=uEchoObjectCodeMin; n<=uEchoTestObjectCodeMax; n++) {
    uEchoObject *obj = uecho_node_getobjectbycode(node, (uEchoObjectCode)n);
    BOOST_CHECK(obj);
    BOOST_CHECK_EQUAL(uecho_object_getcode(obj), n);
    BOOST_CHECK_EQUAL(uecho_object_getparentnode(obj), node);
  }
  
  BOOST_CHECK_EQUAL(uecho_node_getobjectcount(node), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));
  
  uecho_node_delete(node);
}


BOOST_AUTO_TEST_CASE(NodeProfileClass)
{
  uEchoNode *node = uecho_node_new();
  BOOST_CHECK(node);
  
  BOOST_CHECK(uecho_node_setobject(node, 0x001101));
  BOOST_CHECK(uecho_node_setobject(node, 0x001102));
  BOOST_CHECK(uecho_node_setobject(node, 0x001201));

  BOOST_CHECK(uecho_node_hasobjectbycode(node, 0x001101));
  BOOST_CHECK(uecho_node_hasobjectbycode(node, 0x001102));
  BOOST_CHECK(uecho_node_hasobjectbycode(node, 0x001201));

  uEchoObject *obj = uecho_node_getnodeprofileclassobject(node);
  BOOST_CHECK(obj);
  
  BOOST_CHECK_EQUAL(uecho_nodeprofileclass_getclasscount(obj), 3);
  BOOST_CHECK_EQUAL(uecho_nodeprofileclass_getinstancecount(obj), 3);
  
  uecho_node_delete(node);
}
