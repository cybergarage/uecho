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

#include <uecho/core/message.h>

BOOST_AUTO_TEST_CASE(MessageTid)
{
  uEchoMessage *msg = uecho_message_new();
  
  for (int n=0; n<100; n++) {
    BOOST_CHECK(uecho_message_settid(msg, n));
    int tid = uecho_message_gettid(msg);
    BOOST_CHECK_EQUAL(n, tid);
  }
  
  uecho_message_delete(msg);
}
