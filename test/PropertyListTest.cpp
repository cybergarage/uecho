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

#include <stdlib.h>

#include <uecho/_property.h>

BOOST_AUTO_TEST_CASE(PropertyListAdd)
{
  uEchoPropertyList* props = uecho_propertylist_new();

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), 0);

  for (size_t n = uEchoPropertyCodeMin; n <= uEchoPropertyCodeMax; n++) {
    uEchoProperty* prop = uecho_property_new();
    uecho_property_setcode(prop, n);
    byte* prop_data = (byte*)malloc(n);
    uecho_property_setdata(prop, prop_data, n);
    BOOST_REQUIRE(uecho_propertylist_add(props, prop));
    free(prop_data);
  }

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  for (size_t n = uEchoPropertyCodeMin; n <= uEchoPropertyCodeMax; n++) {
    uEchoProperty* prop = uecho_propertylist_findbycode(props, n);
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), n);
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), n);
  }

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  BOOST_REQUIRE(uecho_propertylist_clear(props));

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), 0);

  uecho_propertylist_delete(props);
}

BOOST_AUTO_TEST_CASE(PropertyListSet)
{
  uEchoPropertyList* props = uecho_propertylist_new();

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), 0);

  for (size_t n = uEchoPropertyCodeMin; n <= uEchoPropertyCodeMax; n++) {
    byte* prop_data = (byte*)malloc(n);
    BOOST_REQUIRE(uecho_propertylist_setdata(props, n, prop_data, n));
    free(prop_data);
  }

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  for (size_t n = uEchoPropertyCodeMin; n <= uEchoPropertyCodeMax; n++) {
    uEchoProperty* prop = uecho_propertylist_findbycode(props, n);
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), n);
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), n);
  }

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  BOOST_REQUIRE(uecho_propertylist_clear(props));

  BOOST_REQUIRE_EQUAL(uecho_propertylist_size(props), 0);

  uecho_propertylist_delete(props);
}
