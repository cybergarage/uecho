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
#include <vector>
#include <string>

#include <uecho/property_internal.h>

BOOST_AUTO_TEST_CASE(PropertyBasicFunctions)
{
  uEchoProperty *prop = uecho_property_new();
  
  for (int n=uEchoPropertyCodeMin; n<uEchoPropertyCodeMax; n++) {
      uecho_property_setcode(prop, n);
      BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
  }

  uecho_property_delete(prop);
}

BOOST_AUTO_TEST_CASE(PropertySetData)
{
  uEchoProperty *prop = uecho_property_new();
  
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  uecho_property_cleardata(prop);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);
  
  uecho_property_setdata(prop, NULL, 0);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);
  
  std::vector<std::string> testDataVec;
  testDataVec.push_back("a");
  testDataVec.push_back("abcd");
  testDataVec.push_back("abcd0123456789");
  
  for (int i=0; i<testDataVec.size(); i++) {
    const char *testData = testDataVec[i].c_str();
    size_t testDataLen = strlen(testData);
    uecho_property_setdata(prop, (const byte *)testData, testDataLen);
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), testDataLen);
    byte *propData = uecho_property_getdata(prop);
    BOOST_CHECK(propData);
    for (int n=0; n<testDataLen; n++) {
      BOOST_CHECK_EQUAL(testData[n], propData[n]);
    }
  }
  
  uecho_property_cleardata(prop);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);
  
  uecho_property_delete(prop);
}

BOOST_AUTO_TEST_CASE(PropertyPermission)
{
  uEchoProperty *prop = uecho_property_new();

  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrNone);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  
  uecho_property_setattribute(prop, uEchoPropertyAttrRead);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  
  uecho_property_setattribute(prop, uEchoPropertyAttrWrite);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), true);

  uecho_property_setattribute(prop, uEchoPropertyAttrReadWrite);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);

  uecho_property_delete(prop);
}

BOOST_AUTO_TEST_CASE(PropertyAnnouncement)
{
  uEchoProperty *prop = uecho_property_new();
  
  BOOST_CHECK_EQUAL(uecho_property_isannouncement(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnno);
  BOOST_CHECK_EQUAL(uecho_property_isannouncement(prop), true);
  
  uecho_property_setattribute(prop, uEchoPropertyAttrNone);
  BOOST_CHECK_EQUAL(uecho_property_isannouncement(prop), false);
  
  uecho_property_delete(prop);
}


BOOST_AUTO_TEST_CASE(PropertyByte2Integer)
{
  uEchoProperty *prop = uecho_property_new();
 
  int val;
  for (int n=0; n<=0xFF; n++) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 1));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 1, &val));
    BOOST_CHECK_EQUAL(n, val);
  }
  
  for (int n=0; n<=0xFFFF; n+=(0xFFFF/0xFF)) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 2));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 2, &val));
    BOOST_CHECK_EQUAL(n, val);
  }
  
  for (int n=0; n<=0xFFFFFF; n+=(0xFFFFFF/0xFF)) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 3));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 3, &val));
    BOOST_CHECK_EQUAL(n, val);
  }
  
  for (int n=0; n<0xFFFFFFFF; n+=(0xFFFFFFFF/0xFF)) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 4));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 4, &val));
    BOOST_CHECK_EQUAL(n, val);
  }

  uecho_property_delete(prop);
}

