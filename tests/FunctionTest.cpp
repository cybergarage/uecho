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

BOOST_AUTO_TEST_CASE(ObjCode2ClsCode)
{
  BOOST_CHECK_EQUAL(uecho_classcode_to_classcode(0x0EF001), 0x0EF0);
  BOOST_CHECK_EQUAL(uecho_classcode_to_classcode(0x001102), 0x0011);
  BOOST_CHECK_EQUAL(uecho_classcode_to_classcode(0x001102), 0x0011);
}

