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

#include <uecho/class.h>
#include <uecho/node.h>
#include <uecho/profile.h>

BOOST_AUTO_TEST_CASE(ClassNew)
{
  uEchoClass* cls = uecho_class_new();

  BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), (uEchoClassCode)uEchoClassCodeMin);

  uecho_class_delete(cls);
}

BOOST_AUTO_TEST_CASE(ClassSetCode)
{
  uEchoClass* cls = uecho_class_new();

  uecho_class_setcode(cls, uEchoClassCodeMin);
  BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), (uEchoClassCode)uEchoClassCodeMin);

  uecho_class_setcode(cls, (uEchoClassCodeMin + 1));
  BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), (uEchoClassCode)(uEchoClassCodeMin + 1));

  uecho_class_setcode(cls, (uEchoClassCodeMax / 2));
  BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), (uEchoClassCode)(uEchoClassCodeMax / 2));

  uecho_class_setcode(cls, uEchoClassCodeMax);
  BOOST_REQUIRE_EQUAL(uecho_class_getcode(cls), (uEchoClassCode)uEchoClassCodeMax);

  uecho_class_delete(cls);
}

BOOST_AUTO_TEST_CASE(ClassSetCodes)
{
  uEchoClass* cls = uecho_class_new();

  srand((int)time(NULL));

  for (int n = 0; n < 10; n++) {
    byte code;

    code = rand() % 255;
    uecho_class_setclassgroupcode(cls, code);
    BOOST_REQUIRE_EQUAL(uecho_class_getclassgroupcode(cls), code);

    code = rand() % 255;
    uecho_class_setclasscode(cls, code);
    BOOST_REQUIRE_EQUAL(uecho_class_getclasscode(cls), code);
  }

  uecho_class_delete(cls);
}
