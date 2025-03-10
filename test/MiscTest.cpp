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

#include <uecho/misc.h>

BOOST_AUTO_TEST_CASE(ObjCode2ClsCode)
{
  BOOST_REQUIRE_EQUAL(uecho_objectcode2classcode(0x0EF001), 0x0EF0);
  BOOST_REQUIRE_EQUAL(uecho_objectcode2classcode(0x001102), 0x0011);
  BOOST_REQUIRE_EQUAL(uecho_objectcode2classcode(0x001102), 0x0011);
}

BOOST_AUTO_TEST_CASE(Byte2Integer)
{
  byte int_bytes[4];

  for (int n = 0; n <= 0xFF; n++) {
    uecho_integer2byte(n, int_bytes, 1);
    BOOST_REQUIRE_EQUAL(n, uecho_byte2integer(int_bytes, 1));
  }

  for (int n = 0; n <= 0xFFFF; n += (0xFFFF / 0xFF)) {
    uecho_integer2byte(n, int_bytes, 2);
    BOOST_REQUIRE_EQUAL(n, uecho_byte2integer(int_bytes, 2));
  }

  for (int n = 0; n <= 0xFFFFFF; n += (0xFFFFFF / 0xFF)) {
    uecho_integer2byte(n, int_bytes, 3);
    BOOST_REQUIRE_EQUAL(n, uecho_byte2integer(int_bytes, 3));
  }

  for (int n = 0; n < 0xFFFFFFFF; n += (0xFFFFFFFF / 0xFF)) {
    uecho_integer2byte(n, int_bytes, 4);
    BOOST_REQUIRE_EQUAL(n, uecho_byte2integer(int_bytes, 4));
  }
}

BOOST_AUTO_TEST_CASE(ClassGroupCode)
{
  for (int n = uEchoClassGroupDeviceMin; n <= uEchoClassGroupDeviceMax; n++) {
    BOOST_REQUIRE(uecho_isdeviceclassgroupcode(n));
    BOOST_REQUIRE(!uecho_isprofileclassgroupcode(n));
  }

  BOOST_REQUIRE(!uecho_isdeviceclassgroupcode(uEchoClassGroupProfile));
  BOOST_REQUIRE(uecho_isprofileclassgroupcode(uEchoClassGroupProfile));
}
