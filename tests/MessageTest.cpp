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
  
  uecho_message_settid(msg, UECHO_EHD1);
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), UECHO_EHD1);

  uecho_message_setehd2(msg, UECHO_EHD2);
  BOOST_CHECK_EQUAL(uecho_message_getehd2(msg), UECHO_EHD2);
  
  for (int n=0; n<100; n++) {
  }

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageTid)
{
  uEchoMessage *msg = uecho_message_new();
  
  BOOST_CHECK(uecho_message_settid(msg, 0));
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), 0);
  
  srand((int)time(NULL));
  for (int n=0; n<100; n++) {
    int tid = rand() % UECHO_TID_MAX;
    BOOST_CHECK(uecho_message_settid(msg, tid));
    BOOST_CHECK_EQUAL(uecho_message_gettid(msg), tid);
  }
  
  BOOST_CHECK(uecho_message_settid(msg, UECHO_TID_MAX));
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), UECHO_TID_MAX);
  
  uecho_message_delete(msg);
}
