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
