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

#include <uecho/_class.h>

const int U_ECHO_TEST_CLASS_CODE_MAX = uEchoClassCodeMax % 100;

BOOST_AUTO_TEST_CASE(ClassListAdd)
{
  uEchoClassList* clsses = uecho_classlist_new();

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), 0);

  for (size_t n = uEchoClassCodeMin; n <= U_ECHO_TEST_CLASS_CODE_MAX; n++) {
    uEchoClass* cls = uecho_class_new();
    uecho_class_setcode(cls, (uEchoClassCode)n);
    BOOST_REQUIRE(uecho_classlist_add(clsses, cls));
  }

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), (U_ECHO_TEST_CLASS_CODE_MAX - uEchoClassCodeMin + 1));

  for (size_t n = uEchoClassCodeMin; n <= U_ECHO_TEST_CLASS_CODE_MAX; n++) {
    uEchoClass* cls = uecho_classlist_getbycode(clsses, (uEchoClassCode)n);
    BOOST_REQUIRE(cls);
    BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), n);
  }

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), (U_ECHO_TEST_CLASS_CODE_MAX - uEchoClassCodeMin + 1));

  BOOST_REQUIRE(uecho_classlist_clear(clsses));

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), 0);

  uecho_classlist_delete(clsses);
}

BOOST_AUTO_TEST_CASE(ClassListSet)
{
  uEchoClassList* clsses = uecho_classlist_new();

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), 0);

  for (size_t n = uEchoClassCodeMin; n <= U_ECHO_TEST_CLASS_CODE_MAX; n++) {
    BOOST_REQUIRE(uecho_classlist_set(clsses, (uEchoClassCode)n));
  }

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), (U_ECHO_TEST_CLASS_CODE_MAX - uEchoClassCodeMin + 1));

  for (size_t n = uEchoClassCodeMin; n <= U_ECHO_TEST_CLASS_CODE_MAX; n++) {
    uEchoClass* cls = uecho_classlist_getbycode(clsses, (uEchoClassCode)n);
    BOOST_REQUIRE(cls);
    BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), n);
  }

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), (U_ECHO_TEST_CLASS_CODE_MAX - uEchoClassCodeMin + 1));

  BOOST_REQUIRE(uecho_classlist_clear(clsses));

  BOOST_REQUIRE_EQUAL(uecho_classlist_size(clsses), 0);

  uecho_classlist_delete(clsses);
}
