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
  const char* test_addr = "192.168.0.1";

  uEchoNode* node = uecho_node_new();
  BOOST_REQUIRE(node);

  BOOST_REQUIRE_EQUAL(uecho_node_isaddress(node, test_addr), false);

  uecho_node_setaddress(node, test_addr);
  BOOST_REQUIRE_EQUAL(uecho_node_isaddress(node, test_addr), true);

  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeTID)
{
  uEchoNode* node = uecho_node_new();

  uEchoTID first_tid = uecho_node_getnexttid(node);
  BOOST_REQUIRE(uEchoTidMin <= first_tid);
  BOOST_REQUIRE(first_tid <= uEchoTidMax);

  uEchoTID prev_tid = first_tid;
  for (int n = 0; n < 100; n++) {
    uEchoTID tid = uecho_node_getnexttid(node);
    BOOST_REQUIRE(uEchoTidMin <= tid);
    BOOST_REQUIRE(prev_tid < tid);
    BOOST_REQUIRE(tid <= uEchoTidMax);
  }

  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeSetObjects)
{
  uEchoNode* node = uecho_node_new();
  BOOST_REQUIRE(node);

  const int u_echo_test_object_code_max = uEchoObjectCodeMax % 100;

  uecho_node_clear(node);
  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), 0);

  for (size_t n = uEchoObjectCodeMin; n <= u_echo_test_object_code_max; n++) {
    uecho_node_setobject(node, (uEchoObjectCode)n);
  }

  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), (u_echo_test_object_code_max - uEchoObjectCodeMin + 1));

  for (size_t n = uEchoObjectCodeMin; n <= u_echo_test_object_code_max; n++) {
    uEchoObject* obj = uecho_node_getobjectbycode(node, (uEchoObjectCode)n);
    BOOST_REQUIRE(obj);
    BOOST_REQUIRE_EQUAL(uecho_object_getcode(obj), n);
    BOOST_REQUIRE_EQUAL(uecho_object_getparentnode(obj), node);
  }

  BOOST_REQUIRE_EQUAL(uecho_node_getobjectcount(node), (u_echo_test_object_code_max - uEchoObjectCodeMin + 1));

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

  byte cls_list[] = { 0x02, 0x00, 0x11, 0x00, 0x12 };
  byte* node_cls_list = uecho_nodeprofile_getclasslist(obj);
  BOOST_REQUIRE(node_cls_list);
  for (int n = 0; n < sizeof(cls_list); n++) {
    BOOST_REQUIRE_EQUAL(cls_list[n], node_cls_list[n]);
  }

  // Instance List

  BOOST_REQUIRE_EQUAL(uecho_object_getpropertydatasize(obj, uEchoNodeProfileClassSelfNodeInstanceListS), ((3 * 3) + 1));

  byte ins_list[] = { 0x03, 0x00, 0x11, 0x01, 0x00, 0x11, 0x02, 0x00, 0x12, 0x01 };
  byte* node_ins_list = uecho_nodeprofile_getinstancelist(obj);
  BOOST_REQUIRE(node_ins_list);
  for (int n = 0; n < sizeof(ins_list); n++) {
    BOOST_REQUIRE_EQUAL(ins_list[n], node_ins_list[n]);
  }

  // Notification Instance List

  node_ins_list = uecho_nodeprofile_getnotificationinstancelist(obj);
  BOOST_REQUIRE(node_ins_list);
  for (int n = 0; n < sizeof(ins_list); n++) {
    BOOST_REQUIRE_EQUAL(ins_list[n], node_ins_list[n]);
  }

  uecho_node_delete(node);
}
