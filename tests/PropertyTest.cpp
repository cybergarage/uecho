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
#include <vector>
#include <string>

#include <uecho/property.h>

BOOST_AUTO_TEST_CASE(PropertyBasicFunctions)
{
  uEchoProperty *prop = uecho_property_new();
  
  for (int n=uEchoPropertyCodeMin; n<uEchoPropertyCodeMax; n++)
{
      uecho_property_setcode(prop, n);
      BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
  }

  uecho_property_delete(prop);
}

BOOST_AUTO_TEST_CASE(PropertySetData)
{
  uEchoProperty *prop = uecho_property_new();
  
  BOOST_CHECK_EQUAL(uecho_property_getcount(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  uecho_property_cleardata(prop);
  BOOST_CHECK_EQUAL(uecho_property_getcount(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);
  
  uecho_property_setdata(prop, NULL, 0);
  BOOST_CHECK_EQUAL(uecho_property_getcount(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);
  
  std::vector<std::string> testDataVec;
  testDataVec.push_back("a");
  testDataVec.push_back("abcd");
  testDataVec.push_back("abcd0123456789");
  
  for (int i=0; i<testDataVec.size(); i++)
{
    const char *testData = testDataVec[i].c_str();
    size_t testDataLen = strlen(testData);
    uecho_property_setdata(prop, (const byte *)testData, testDataLen);
    BOOST_CHECK_EQUAL(uecho_property_getcount(prop), testDataLen);
    byte *propData = uecho_property_getdata(prop);
    BOOST_CHECK(propData);
    for (int n=0; n<testDataLen; n++)
{
      BOOST_CHECK_EQUAL(testData[n], propData[n]);
    }
  }
  
  uecho_property_cleardata(prop);
  BOOST_CHECK_EQUAL(uecho_property_getcount(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);
  
  uecho_property_delete(prop);
}
