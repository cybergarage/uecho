/************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ************************************************************/

#define BOOST_TEST_MODULE uEchoTest
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <uecho/util/log.h>

struct uEchoFixture {
  uEchoFixture()
  {
    uecho_log_setlevel(UECHO_LOG_DEBUG);
  }
  ~uEchoFixture()
  {
  }
};

BOOST_GLOBAL_FIXTURE(uEchoFixture);
