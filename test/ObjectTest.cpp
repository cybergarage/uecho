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
#include <uecho/object.h>
#include <uecho/profile.h>

BOOST_AUTO_TEST_CASE(ObjectNew)
{
  uEchoObject* obj = uecho_object_new();

  BOOST_CHECK_EQUAL(uecho_object_getcode(obj), (uEchoObjectCode)uEchoObjectCodeMin);
  BOOST_CHECK(!uecho_object_getparentnode(obj));

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(ObjectSetCode)
{
  uEchoObject* obj = uecho_object_new();

  uecho_object_setcode(obj, uEchoObjectCodeMin);
  BOOST_CHECK_EQUAL(uecho_object_getcode(obj), (uEchoObjectCode)uEchoObjectCodeMin);

  uecho_object_setcode(obj, (uEchoObjectCodeMin + 1));
  BOOST_CHECK_EQUAL(uecho_object_getcode(obj), (uEchoObjectCode)(uEchoObjectCodeMin + 1));

  uecho_object_setcode(obj, (uEchoObjectCodeMax / 2));
  BOOST_CHECK_EQUAL(uecho_object_getcode(obj), (uEchoObjectCode)(uEchoObjectCodeMax / 2));

  uecho_object_setcode(obj, uEchoObjectCodeMax);
  BOOST_CHECK_EQUAL(uecho_object_getcode(obj), (uEchoObjectCode)uEchoObjectCodeMax);

  uecho_object_setcode(obj, uEchoNodeProfileObject);
  BOOST_CHECK_EQUAL(uecho_object_getcode(obj), (uEchoObjectCode)uEchoNodeProfileObject);

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(ObjectSetCodes)
{
  uEchoObject* obj = uecho_object_new();

  srand((int)time(NULL));

  for (int n = 0; n < 10; n++) {
    byte code;

    code = rand() % 255;
    uecho_object_setgroupcode(obj, code);
    BOOST_CHECK_EQUAL(uecho_object_getgroupcode(obj), code);

    code = rand() % 255;
    uecho_object_setclasscode(obj, code);
    BOOST_CHECK_EQUAL(uecho_object_getclasscode(obj), code);

    code = rand() % 255;
    uecho_object_setinstancecode(obj, code);
    BOOST_CHECK_EQUAL(uecho_object_getinstancecode(obj), code);
  }

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(ObjectSetProperty)
{
  uEchoObject* obj = uecho_object_new();

  uecho_object_clearproperties(obj);

  BOOST_CHECK_EQUAL(uecho_object_getpropertycount(obj), 0);

  for (size_t n = uEchoPropertyCodeMin; n <= uEchoPropertyCodeMax; n++) {
    byte* prop_data = (byte*)malloc(n);
    BOOST_CHECK(prop_data);
    BOOST_CHECK(uecho_object_setpropertydata(obj, n, prop_data, n));
    free(prop_data);
  }

  BOOST_CHECK_EQUAL(uecho_object_getpropertycount(obj), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  for (size_t n = uEchoPropertyCodeMin; n <= uEchoPropertyCodeMax; n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, n);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), n);
  }

  BOOST_CHECK_EQUAL(uecho_object_getpropertycount(obj), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(ObjectMandatoryProperties)
{
  uEchoObject* obj = uecho_object_new();

  // Propery mandatory properties

  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileManufacturerCode));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileAnnoPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileSetPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileGetPropertyMap));

  uecho_object_delete(obj);
}
