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
#include <string>
#include <vector>

#include <uecho/_property.h>
#if defined(UECHO_TEST_ALLOCATOR_FAILURES)
#include <uecho/_controller.h>
#include <uecho/net/interface.h>
#include <uecho/profile.h>
#endif

BOOST_AUTO_TEST_CASE(PropertyBasicFunctions)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_REQUIRE(prop);

  for (int n = uEchoPropertyCodeMin; n < uEchoPropertyCodeMax; n++) {
    uecho_property_setcode(prop, n);
    BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), n);
  }

  BOOST_REQUIRE(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertySetData)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_REQUIRE_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  uecho_property_cleardata(prop);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_REQUIRE_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  uecho_property_setdata(prop, NULL, 0);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_REQUIRE_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  std::vector<std::string> testDataVec;
  testDataVec.push_back("a");
  testDataVec.push_back("abcd");
  testDataVec.push_back("abcd0123456789");

  for (int i = 0; i < testDataVec.size(); i++) {
    const char* testData = testDataVec[i].c_str();
    size_t testDataLen = strlen(testData);
    uecho_property_setdata(prop, (const byte*)testData, testDataLen);
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), testDataLen);
    BOOST_REQUIRE(uecho_property_isdataequal(prop, (const byte*)testData, testDataLen));
    byte* propData = uecho_property_getdata(prop);
    BOOST_REQUIRE(propData);
    for (int n = 0; n < testDataLen; n++) {
      BOOST_REQUIRE_EQUAL(testData[n], propData[n]);
    }
  }

  uecho_property_cleardata(prop);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_REQUIRE_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  BOOST_REQUIRE(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyPermission)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrNone);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrRead);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrReadRequired);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrWrite);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrWriteRequired);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrReadWrite);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnno);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnnoRequired);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), true);
  BOOST_REQUIRE_EQUAL(uecho_property_isannouncerequired(prop), true);

  BOOST_REQUIRE(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyAnnouncement)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnno);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), true);

  uecho_property_setattribute(prop, uEchoPropertyAttrNone);
  BOOST_REQUIRE_EQUAL(uecho_property_isannounceable(prop), false);

  BOOST_REQUIRE(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyByte2Integer)
{
  uEchoProperty* prop = uecho_property_new();

  int val;
  for (int n = 0; n <= 0xFF; n++) {
    BOOST_REQUIRE(uecho_property_setintegerdata(prop, n, 1));
    BOOST_REQUIRE(uecho_property_getintegerdata(prop, &val));
    BOOST_REQUIRE_EQUAL(n, val);
  }

  for (int n = 0; n <= 0xFFFF; n += (0xFFFF / 0xFF)) {
    BOOST_REQUIRE(uecho_property_setintegerdata(prop, n, 2));
    BOOST_REQUIRE(uecho_property_getintegerdata(prop, &val));
    BOOST_REQUIRE_EQUAL(n, val);
  }

  for (int n = 0; n <= 0xFFFFFF; n += (0xFFFFFF / 0xFF)) {
    BOOST_REQUIRE(uecho_property_setintegerdata(prop, n, 3));
    BOOST_REQUIRE(uecho_property_getintegerdata(prop, &val));
    BOOST_REQUIRE_EQUAL(n, val);
  }

  for (int n = 0; n < 0xFFFFFFFF; n += (0xFFFFFFFF / 0xFF)) {
    BOOST_REQUIRE(uecho_property_setintegerdata(prop, n, 4));
    BOOST_REQUIRE(uecho_property_getintegerdata(prop, &val));
    BOOST_REQUIRE_EQUAL(n, val);
  }

  BOOST_REQUIRE(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyAddData)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);

  const size_t propertyAddDataCount = 10;

  for (size_t n = 0; n < propertyAddDataCount; n++) {
    byte newByte = n;
    BOOST_REQUIRE(uecho_property_addbytedata(prop, newByte));
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), (n + 1));
    byte* bytes = uecho_property_getdata(prop);
    BOOST_REQUIRE(bytes);
    BOOST_REQUIRE_EQUAL(bytes[n], newByte);
  }

  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), propertyAddDataCount);

  BOOST_REQUIRE(uecho_property_delete(prop));
}

#if defined(UECHO_TEST_ALLOCATOR_FAILURES)
// GNU --wrap affects library references only. Failure state is thread-local,
// so receive workers cannot consume a scheduled failure.

struct TestAllocationState {
  int mallocAfter = 0;
  bool calloc = false;
  bool realloc = false;
};

static TestAllocationState& uecho_test_allocationstate()
{
  static thread_local TestAllocationState state;
  return state;
}

extern "C" void* uecho_test_real_malloc(size_t) asm("__real_malloc");
extern "C" void* uecho_test_real_calloc(size_t, size_t) asm("__real_calloc");
extern "C" void* uecho_test_real_realloc(void*, size_t) asm("__real_realloc");

extern "C" void* uecho_test_malloc(size_t) asm("__wrap_malloc");

extern "C" void* uecho_test_malloc(size_t size)
{
  if (auto& state = uecho_test_allocationstate(); state.mallocAfter > 0) {
    --state.mallocAfter;
    if (state.mallocAfter == 0)
      return nullptr;
  }
  return uecho_test_real_malloc(size);
}

extern "C" void* uecho_test_calloc(size_t, size_t) asm("__wrap_calloc");

extern "C" void* uecho_test_calloc(size_t count, size_t size)
{
  if (uecho_test_allocationstate().calloc) {
    uecho_test_allocationstate().calloc = false;
    return nullptr;
  }
  return uecho_test_real_calloc(count, size);
}

extern "C" void* uecho_test_realloc(void*, size_t) asm("__wrap_realloc");

extern "C" void* uecho_test_realloc(void* data, size_t size)
{
  if (uecho_test_allocationstate().realloc) {
    uecho_test_allocationstate().realloc = false;
    return nullptr;
  }
  return uecho_test_real_realloc(data, size);
}

BOOST_AUTO_TEST_CASE(AllocationFailureCleanup)
{
  uecho_test_allocationstate().mallocAfter = 2; // Node allocation succeeds; its mutex allocation fails.
  const uEchoNode* node = uecho_node_new();
  BOOST_CHECK(!node);
  uecho_test_allocationstate().mallocAfter = 1; // Controller calloc succeeds; its mutex allocation fails.
  const uEchoController* failedController = uecho_controller_new();
  BOOST_CHECK(!failedController);

  uEchoController* controller = uecho_controller_new();
  uEchoMessage* search = uecho_message_search_new();
  byte emptyList = 0;
  uecho_message_setproperty(search, uEchoNodeProfileClassSelfNodeInstanceListS, &emptyList, 1);
  uecho_message_setesv(search, uEchoEsvReadResponse);
  uecho_message_setsourceaddress(search, UECHO_NET_IPV4_LOOPBACK);
  uecho_test_allocationstate().mallocAfter = 1; // Discovery cannot allocate a peer.
  uecho_controller_servermessagelistener(controller, search);
  BOOST_CHECK_EQUAL(uecho_controller_getnodecount(controller), 0);
  // A subsequent discovery proves the failure path released its mutex.
  uecho_controller_servermessagelistener(controller, search);
  BOOST_CHECK_EQUAL(uecho_controller_getnodecount(controller), 1);

  uEchoMessage* copy = uecho_message_new();
  uEchoMessage* request = uecho_message_search_new();
  uecho_test_allocationstate().mallocAfter = 1; // The first allocation is the copied property.
  bool copied = uecho_message_set(copy, request);
  BOOST_CHECK(!copied);
  BOOST_CHECK(uecho_message_set(copy, request));

  uEchoProperty* property = uecho_property_new();
  byte value = 0x30;
  uecho_test_allocationstate().calloc = true;
  bool stored = uecho_property_setdata(property, &value, 1);
  BOOST_CHECK(!stored);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(property), 0);
  BOOST_CHECK(uecho_property_setdata(property, &value, 1));
  uecho_test_allocationstate().realloc = true;
  bool appended = uecho_property_adddata(property, &value, 1);
  BOOST_CHECK(!appended);
  byte preserved = 0;
  BOOST_REQUIRE(uecho_property_getbytedata(property, &preserved));
  BOOST_CHECK_EQUAL(preserved, value);
  BOOST_CHECK(uecho_property_adddata(property, &value, 1));
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(property), 2);
  uecho_property_delete(property);
  uecho_message_delete(copy);
  uecho_message_delete(request);
  uecho_message_delete(search);
  uecho_controller_delete(controller);
}
#endif
