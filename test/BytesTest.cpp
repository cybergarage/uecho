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

#include <uecho/util/bytes.h>

BOOST_AUTO_TEST_CASE(Bytes)
{
  byte* ibytes;
  int n;

  for (n=0; n<=0xFF; n++) {
    ibytes = uecho_bytes_fromint(n, 1);
    BOOST_CHECK_EQUAL(uecho_bytes_toint(ibytes, 1), n);
    free(ibytes);
  }

  for (n=0; n<=0xFFFF; n+=(0xFFFF / 0xFF)) {
    ibytes = uecho_bytes_fromint(n, 2);
    BOOST_CHECK_EQUAL(uecho_bytes_toint(ibytes, 2), n);
    free(ibytes);
  }

  for (n=0; n<=0xFFFFFF; n+=(0xFFFFFF / 0xFFFF)) {
    ibytes = uecho_bytes_fromint(n, 3);
    BOOST_CHECK_EQUAL(uecho_bytes_toint(ibytes, 3), n);
    free(ibytes);
  }
}
