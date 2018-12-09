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

#include <uecho/object_internal.h>

const int uEchoTestObjectCodeMax = uEchoObjectCodeMax % 100;

BOOST_AUTO_TEST_CASE(ObjectListAdd)
{
  uEchoObjectList* objs = uecho_objectlist_new();

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), 0);

  for (size_t n = uEchoObjectCodeMin; n <= uEchoTestObjectCodeMax; n++) {
    uEchoObject* obj = uecho_object_new();
    uecho_object_setcode(obj, (uEchoObjectCode)n);
    BOOST_CHECK(uecho_objectlist_add(objs, obj));
  }

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));

  for (size_t n = uEchoObjectCodeMin; n <= uEchoTestObjectCodeMax; n++) {
    uEchoObject* obj = uecho_objectlist_getbycode(objs, (uEchoObjectCode)n);
    BOOST_CHECK(obj);
    BOOST_CHECK_EQUAL(uecho_object_getcode(obj), n);
  }

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));

  BOOST_CHECK(uecho_objectlist_clear(objs));

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), 0);

  uecho_objectlist_delete(objs);
}

BOOST_AUTO_TEST_CASE(ObjectListSet)
{
  uEchoObjectList* objs = uecho_objectlist_new();

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), 0);

  for (size_t n = uEchoObjectCodeMin; n <= uEchoTestObjectCodeMax; n++) {
    BOOST_CHECK(uecho_objectlist_set(objs, (uEchoObjectCode)n));
  }

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));

  for (size_t n = uEchoObjectCodeMin; n <= uEchoTestObjectCodeMax; n++) {
    uEchoObject* obj = uecho_objectlist_getbycode(objs, (uEchoObjectCode)n);
    BOOST_CHECK(obj);
    BOOST_CHECK_EQUAL(uecho_object_getcode(obj), n);
  }

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), (uEchoTestObjectCodeMax - uEchoObjectCodeMin + 1));

  BOOST_CHECK(uecho_objectlist_clear(objs));

  BOOST_CHECK_EQUAL(uecho_objectlist_size(objs), 0);

  uecho_objectlist_delete(objs);
}
