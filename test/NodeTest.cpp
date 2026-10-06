/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <algorithm>
#include <array>
#include <atomic>
#include <boost/test/unit_test.hpp>
#include <future>
#include <uecho/_node.h>
#include <uecho/profile.h>

extern "C" int uecho_node_handlerequestmessage(uEchoObject*, uEchoEsv, byte, uEchoProperty**, bool (*)(uEchoMessage*, uEchoProperty*), uEchoMessage*);

static int uecho_test_propertyrequest(uEchoObject* object, uEchoEsv esv, uEchoMessage* request, uEchoMessage* response)
{
  return uecho_node_handlerequestmessage(object, esv, request->opc, request->ep, uecho_message_addproperty, response);
}

BOOST_AUTO_TEST_CASE(NodePropertyServicePermissions)
{
  uEchoNode* node = uecho_node_new();
  uEchoObject* object = uecho_node_getnodeprofileclassobject(node);
  byte original = 0x30;
  byte replacement = 0x31;
  uecho_object_setproperty(object, 0xE0, uEchoPropertyAttrReadWrite);
  uecho_object_setproperty(object, 0xE1, uEchoPropertyAttrReadRequired);
  uecho_object_setproperty(object, 0xE2, uEchoPropertyAttrWriteRequired);
  for (byte code : { byte(0xE0), byte(0xE1), byte(0xE2) })
    uecho_object_setpropertybytedata(object, code, original);

  for (uEchoEsv esv : { uEchoEsv(uEchoEsvWriteRequest), uEchoEsv(uEchoEsvWriteRequestResponseRequired) }) {
    uEchoMessage* request = uecho_message_new();
    uEchoMessage* response = uecho_message_new();
    uecho_message_setproperty(request, 0xE0, &replacement, 1);
    uecho_message_setproperty(request, 0xE1, &replacement, 1);
    uecho_message_setproperty(request, 0xE2, &replacement, 1);
    BOOST_CHECK_EQUAL(uecho_test_propertyrequest(object, esv, request, response), 2);
    byte value;
    BOOST_REQUIRE(uecho_object_getpropertybytedata(object, 0xE1, &value));
    BOOST_CHECK_EQUAL(value, original);
    BOOST_REQUIRE(uecho_object_getpropertybytedata(object, 0xE0, &value));
    BOOST_CHECK_EQUAL(value, replacement);
    uecho_message_delete(request);
    uecho_message_delete(response);
  }
  for (uEchoEsv esv : { uEchoEsv(uEchoEsvReadRequest), uEchoEsv(uEchoEsvNotificationRequest), uEchoEsv(uEchoEsvNotificationResponseRequired) }) {
    uEchoMessage* request = uecho_message_new();
    uEchoMessage* response = uecho_message_new();
    for (byte code : { byte(0xE0), byte(0xE1), byte(0xE2) })
      uecho_message_setproperty(request, code, nullptr, 0);
    BOOST_CHECK_EQUAL(uecho_test_propertyrequest(object, esv, request, response), 2);
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(uecho_message_getpropertybycode(response, 0xE2)), 0);
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(uecho_message_getpropertybycode(response, 0xE0)), 1);
    uecho_message_delete(request);
    uecho_message_delete(response);
  }
  uEchoMessage* request = uecho_message_new();
  uEchoMessage* response = uecho_message_new();
  uEchoProperty* version = uecho_object_getproperty(object, 0x82);
  uEchoProperty* before = uecho_property_copy(version);
  uecho_message_setproperty(request, 0x82, &replacement, 1);
  BOOST_CHECK_EQUAL(uecho_test_propertyrequest(object, uEchoEsvWriteRequestResponseRequired, request, response), 0);
  BOOST_CHECK(uecho_property_equals(before, version));
  uecho_property_delete(before);
  uecho_message_delete(request);
  uecho_message_delete(response);
  uecho_node_delete(node);
}

BOOST_AUTO_TEST_CASE(NodeConcurrentPropertyMapReaders)
{
  uEchoNode* node = uecho_node_new();
  uEchoProperty* property = uecho_object_getproperty(uecho_node_getnodeprofileclassobject(node), uEchoObjectGetPropertyMap);
  BOOST_REQUIRE(property);
  std::array<byte, 2> map = { 1, 0xE0 };
  uecho_property_setdata(property, map.data(), map.size());
  uEchoProperty* reference = uecho_property_copy(property);
  BOOST_REQUIRE(reference);
  std::atomic_bool valid(true);
  auto writer = std::async(std::launch::async, [property, map] {
    std::array<byte, 1> empty = { 0 };
    for (int n = 0; n < 4000; n++)
      uecho_property_setdata(property, n % 2 ? map.data() : empty.data(), n % 2 ? map.size() : empty.size());
  });
  auto reader = std::async(std::launch::async, [property, reference, &valid] {
    for (int n = 0; n < 4000; n++) {
      if (size_t size = uecho_property_getdatasize(property); size < 1 || size > 2)
        valid = false;
      if (size_t count = 0; !uecho_property_getpropertymapcount(property, &count) || count > 1)
        valid = false;
      if (uEchoPropertyCode code = 0; uecho_property_getpropertymapcodes(property, &code, 1) && code != 0xE0)
        valid = false;
      uecho_property_getpropertymapcodes(property, nullptr, 0);
      uecho_property_equals(property, reference);
    }
  });
  writer.get();
  reader.get();
  BOOST_CHECK(valid.load());
  uecho_property_delete(reference);
  uecho_node_delete(node);
}

static bool uecho_test_consistentsnapshot(uEchoProperty* property)
{
  uEchoProperty* snapshot = uecho_property_copy(property);
  const size_t length = uecho_property_getdatasize(snapshot);
  const byte* data = uecho_property_getdata(snapshot);
  bool valid = length == 0 || length == 1 || length == 64;
  if (valid && length > 0)
    valid = std::all_of(data, data + length, [length](byte value) { return value == (length == 1 ? 0x31 : 0x64); });
  uecho_property_delete(snapshot);
  return valid;
}

BOOST_AUTO_TEST_CASE(NodeConcurrentPropertyRequests)
{
  uEchoNode* node = uecho_node_new();
  uEchoObject* object = uecho_node_getnodeprofileclassobject(node);
  uecho_object_setproperty(object, 0xE0, uEchoPropertyAttrReadWrite);
  uEchoProperty* property = uecho_object_getproperty(object, 0xE0);
  std::atomic_bool valid(true);
  auto writer = [object, &valid](size_t length, byte value) {
    std::array<byte, 64> data;
    data.fill(value);
    for (int n = 0; n < 2000; n++) {
      uEchoMessage* request = uecho_message_new();
      uEchoMessage* response = uecho_message_new();
      uecho_message_setproperty(request, 0xE0, data.data(), length);
      if (uecho_test_propertyrequest(object, uEchoEsvWriteRequest, request, response) != 1)
        valid = false;
      uecho_message_delete(request);
      uecho_message_delete(response);
    }
  };
  auto reader = [object, property, &valid]() {
    for (int n = 0; n < 2000; n++) {
      if (!uecho_test_consistentsnapshot(property))
        valid = false;
      uEchoMessage* request = uecho_message_new();
      uEchoMessage* response = uecho_message_new();
      uecho_message_setproperty(request, 0xE0, nullptr, 0);
      if (uecho_test_propertyrequest(object, uEchoEsvReadRequest, request, response) != 1)
        valid = false;
      uecho_message_delete(request);
      uecho_message_delete(response);
    }
  };
  auto first = std::async(std::launch::async, writer, 64, 0x64);
  auto second = std::async(std::launch::async, writer, 1, 0x31);
  auto third = std::async(std::launch::async, reader);
  first.get();
  second.get();
  third.get();
  BOOST_CHECK(valid.load());
  uecho_node_delete(node);
}

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
