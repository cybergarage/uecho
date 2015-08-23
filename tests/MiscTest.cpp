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
  BOOST_CHECK_EQUAL(uecho_objectcode2classcode(0x0EF001), 0x0EF0);
  BOOST_CHECK_EQUAL(uecho_objectcode2classcode(0x001102), 0x0011);
  BOOST_CHECK_EQUAL(uecho_objectcode2classcode(0x001102), 0x0011);
}

void uecho_integer2byte(int val, byte *bytes, size_t byteSize);
int uecho_byte2integer(byte *bytes, size_t byteSize);

BOOST_AUTO_TEST_CASE(Byte2Integer)
{
  byte intBytes[4];
  
  for (int n=0; n<0xFF; n++) {
    uecho_integer2byte(n, intBytes, 1);
    BOOST_CHECK_EQUAL(n, uecho_byte2integer(intBytes, 1));
  }

  for (int n=0; n<0xFFFF; n+=(0xFFFF/0xFF)) {
    uecho_integer2byte(n, intBytes, 2);
    BOOST_CHECK_EQUAL(n, uecho_byte2integer(intBytes, 2));
  }

  for (int n=0; n<0xFFFFFF; n+=(0xFFFFFF/0xFF)) {
    uecho_integer2byte(n, intBytes, 3);
    BOOST_CHECK_EQUAL(n, uecho_byte2integer(intBytes, 3));
  }

  for (int n=0; n<0xFFFFFFFF; n+=(0xFFFFFFFF/0xFF)) {
    uecho_integer2byte(n, intBytes, 4);
    BOOST_CHECK_EQUAL(n, uecho_byte2integer(intBytes, 4));
  }
}

