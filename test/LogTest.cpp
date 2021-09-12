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

#include <uecho/util/log.h>

BOOST_AUTO_TEST_CASE(LogFunction)
{
  const char* msg = "hello";

  uecho_log_debug("%s", msg);
  uecho_log_warn("%s", msg);
  uecho_log_error("%s", msg);
  uecho_log_info("%s", msg);
}
