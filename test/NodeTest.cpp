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
#include <uecho/_node.h>
#include <uecho/profile.h>

BOOST_AUTO_TEST_CASE(NodeDefault)
{
  uEchoNode* node = uecho_node_new();
  BOOST_REQUIRE(node);

  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), 1);
  BOOST_REQUIRE(uecho_node_hasobjectbycode(node, uEchoNodeProfileObject));

  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeAddress)
{
  const char* testAddr = "192.168.0.1";

  uEchoNode* node = uecho_node_new();
  BOOST_REQUIRE(node);

  BOOST_REQUIRE_EQUAL(uecho_node_isaddress(node, testAddr), false);

  uecho_node_setaddress(node, testAddr);
  BOOST_REQUIRE_EQUAL(uecho_node_isaddress(node, testAddr), true);

  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeTID)
{
  uEchoNode* node = uecho_node_new();

  uEchoTID firstTid = uecho_node_getnexttid(node);
  BOOST_REQUIRE(uEchoTidMin <= firstTid);
  BOOST_REQUIRE(firstTid <= uEchoTidMax);

  uEchoTID prevTid = firstTid;
  for (int n = 0; n < 100; n++) {
    uEchoTID tid = uecho_node_getnexttid(node);
    BOOST_REQUIRE(uEchoTidMin <= tid);
    BOOST_REQUIRE(prevTid < tid);
    BOOST_REQUIRE(tid <= uEchoTidMax);
  }

  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeSetObjects)
{
  uEchoNode* node = uecho_node_new();
  BOOST_REQUIRE(node);

  const int uEchoTestObjectCodeMax = uEchoObjectCodeMax % 100;

  uecho_node_clear(node);
  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), 0);

  for (size_t n = uEchoObjectCodeMin; n <= uEchoTestObjectCodeMax; n++) {
    uecho_node_setobject(node, (uEchoObjectCode)n);
  }

  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));

  for (size_t n = uEchoObjectCodeMin; n <= uEchoTestObjectCodeMax; n++) {
    uEchoObject* obj = uecho_node_getobjectbycode(node, (uEchoObjectCode)n);
    BOOST_REQUIRE(obj);
    BOOST_REQUIRE_EQUAL(uecho_object_getcode(obj), n);
    BOOST_REQUIRE_EQUAL(uecho_object_getparentnode(obj), node);
  }

  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));

  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeProfileClass)
{
  uEchoNode* node = uecho_node_new();
  BOOST_REQUIRE(node);

  BOOST_REQUIRE(uecho_node_setobject(node, 0x001101));
  BOOST_REQUIRE(uecho_node_setobject(node, 0x001102));
  BOOST_REQUIRE(uecho_node_setobject(node, 0x001201));

  BOOST_REQUIRE(uecho_node_hasobjectbycode(node, 0x001101));
  BOOST_REQUIRE(uecho_node_hasobjectbycode(node, 0x001102));
  BOOST_REQUIRE(uecho_node_hasobjectbycode(node, 0x001201));

  uEchoObject* obj = uecho_node_getnodeprofileclassobject(node);
  BOOST_REQUIRE(obj);

  BOOST_REQUIRE_EQUAL(uecho_nodeprofile_getclasscount(obj), 3);
  BOOST_REQUIRE_EQUAL(uecho_nodeprofile_getinstancecount(obj), 3);

  // Class List

  BOOST_REQUIRE_EQUAL(uecho_object_getpropertydatasize(obj, uEchoNodeProfileClassSelfNodeClassListS), ((2 * 2) + 1));

  byte clsList[] = { 0x02, 0x00, 0x11, 0x00, 0x12 };
  byte* nodeClsList = uecho_nodeprofile_getclasslist(obj);
  BOOST_REQUIRE(nodeClsList);
  for (int n = 0; n < sizeof(clsList); n++) {
    BOOST_REQUIRE_EQUAL(clsList[n], nodeClsList[n]);
  }

  // Instance List

  BOOST_REQUIRE_EQUAL(uecho_object_getpropertydatasize(obj, uEchoNodeProfileClassSelfNodeInstanceListS), ((3 * 3) + 1));

  byte insList[] = { 0x03, 0x00, 0x11, 0x01, 0x00, 0x11, 0x02, 0x00, 0x12, 0x01 };
  byte* nodeInsList = uecho_nodeprofile_getinstancelist(obj);
  BOOST_REQUIRE(nodeInsList);
  for (int n = 0; n < sizeof(insList); n++) {
    BOOST_REQUIRE_EQUAL(insList[n], nodeInsList[n]);
  }

  // Notification Instance List

  nodeInsList = uecho_nodeprofile_getnotificationinstancelist(obj);
  BOOST_REQUIRE(nodeInsList);
  for (int n = 0; n < sizeof(insList); n++) {
    BOOST_REQUIRE_EQUAL(insList[n], nodeInsList[n]);
  }

  uecho_node_delete(node);
}
