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

#include <uecho/property.h>

BOOST_AUTO_TEST_CASE(PropertyListAdd)
{
  uEchoPropertyList *props = uecho_propertylist_new();
  
  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), 0);
  
  for (size_t n=uEchoPropertyCodeMin; n<=uEchoPropertyCodeMax; n++) {
    uEchoProperty *prop = uecho_property_new();
    uecho_property_setcode(prop, n);
    byte *propData = (byte *)malloc(n);
    uecho_property_setdata(prop, propData, n);
    BOOST_CHECK(uecho_propertylist_add(props, prop));
    free(propData);
  }

  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));
  
  for (size_t n=uEchoPropertyCodeMin; n<=uEchoPropertyCodeMax; n++) {
    uEchoProperty *prop = uecho_propertylist_getbycode(props, n);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
    BOOST_CHECK_EQUAL(uecho_property_getcount(prop), n);
  }

  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  BOOST_CHECK(uecho_propertylist_clear(props));
  
  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), 0);

  uecho_propertylist_delete(props);
}

BOOST_AUTO_TEST_CASE(PropertyListSet)
{
  uEchoPropertyList *props = uecho_propertylist_new();
  
  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), 0);
  
  for (size_t n=uEchoPropertyCodeMin; n<=uEchoPropertyCodeMax; n++) {
    byte *propData = (byte *)malloc(n);
    BOOST_CHECK(uecho_propertylist_set(props, n, propData, n));
    free(propData);
  }
  
  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));

  for (size_t n=uEchoPropertyCodeMin; n<=uEchoPropertyCodeMax; n++) {
    uEchoProperty *prop = uecho_propertylist_getbycode(props, n);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
    BOOST_CHECK_EQUAL(uecho_property_getcount(prop), n);
  }
  
  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), (uEchoPropertyCodeMax - uEchoPropertyCodeMin + 1));
  
  BOOST_CHECK(uecho_propertylist_clear(props));
  
  BOOST_CHECK_EQUAL(uecho_propertylist_size(props), 0);
  
  uecho_propertylist_delete(props);
}
