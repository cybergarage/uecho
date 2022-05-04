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

#include <uecho/std/database.h>

BOOST_AUTO_TEST_CASE(StandardDatabase)
{
  uEchoDatabase *db = uecho_standard_getdatabase();
  BOOST_CHECK(db);
}
