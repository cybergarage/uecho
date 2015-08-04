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

BOOST_AUTO_TEST_CASE(MessageBasicFunctions)
{
  uEchoMessage *msg = uecho_message_new();
  
  uecho_message_settid(msg, uEchoEhd1);
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), uEchoEhd1);

  uecho_message_setehd2(msg, uEchoEhd2);
  BOOST_CHECK_EQUAL(uecho_message_getehd2(msg), uEchoEhd2);
  
  for (int n=0; n<100; n++) {
  }

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageTid)
{
  uEchoMessage *msg = uecho_message_new();
  
  BOOST_CHECK(uecho_message_settid(msg, uEchoTidMin));
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), uEchoTidMin);
  
  BOOST_CHECK(uecho_message_settid(msg, uEchoTidMax));
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), uEchoTidMax);
  
  srand((int)time(NULL));
  for (int n=0; n<100; n++) {
    int tid = rand() % uEchoTidMax;
    BOOST_CHECK(uecho_message_settid(msg, tid));
    BOOST_CHECK_EQUAL(uecho_message_gettid(msg), tid);
  }
  
  uecho_message_delete(msg);
}
