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

#include <uecho/_object.h>

const int U_ECHO_TEST_OBJECT_CODE_MAX = uEchoObjectCodeMax % 100;

BOOST_AUTO_TEST_CASE(ObjectListAdd)
{
  uEchoObjectList* objs = uecho_objectlist_new();

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), 0);

  for (size_t n = uEchoObjectCodeMin; n <= U_ECHO_TEST_OBJECT_CODE_MAX; n++) {
    uEchoObject* obj = uecho_object_new();
    uecho_object_setcode(obj, (uEchoObjectCode)n);
    BOOST_REQUIRE(uecho_objectlist_add(objs, obj));
  }

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), (U_ECHO_TEST_OBJECT_CODE_MAX - uEchoObjectCodeMin + 1));

  for (size_t n = uEchoObjectCodeMin; n <= U_ECHO_TEST_OBJECT_CODE_MAX; n++) {
    uEchoObject* obj = uecho_objectlist_getbycode(objs, (uEchoObjectCode)n);
    BOOST_REQUIRE(obj);
    BOOST_REQUIRE_EQUAL(uecho_object_getcode(obj), n);
  }

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), (U_ECHO_TEST_OBJECT_CODE_MAX - uEchoObjectCodeMin + 1));

  BOOST_REQUIRE(uecho_objectlist_clear(objs));

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), 0);

  uecho_objectlist_delete(objs);
}

BOOST_AUTO_TEST_CASE(ObjectListSet)
{
  uEchoObjectList* objs = uecho_objectlist_new();

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), 0);

  for (size_t n = uEchoObjectCodeMin; n <= U_ECHO_TEST_OBJECT_CODE_MAX; n++) {
    BOOST_REQUIRE(uecho_objectlist_set(objs, (uEchoObjectCode)n));
  }

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), (U_ECHO_TEST_OBJECT_CODE_MAX - uEchoObjectCodeMin + 1));

  for (size_t n = uEchoObjectCodeMin; n <= U_ECHO_TEST_OBJECT_CODE_MAX; n++) {
    uEchoObject* obj = uecho_objectlist_getbycode(objs, (uEchoObjectCode)n);
    BOOST_REQUIRE(obj);
    BOOST_REQUIRE_EQUAL(uecho_object_getcode(obj), n);
  }

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), (U_ECHO_TEST_OBJECT_CODE_MAX - uEchoObjectCodeMin + 1));

  BOOST_REQUIRE(uecho_objectlist_clear(objs));

  BOOST_REQUIRE_EQUAL(uecho_objectlist_size(objs), 0);

  uecho_objectlist_delete(objs);
}
