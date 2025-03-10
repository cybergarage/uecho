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
#include <boost/thread.hpp>

#include <uecho/util/thread.h>
#include <uecho/util/timer.h>

const int THREAD_TEST_LOOP_NUM = 10;

void test_theread_func(uEchoThread* thread)
{
  int* test_counter = (int*)uecho_thread_getuserdata(thread);
  for (int n = 0; n < THREAD_TEST_LOOP_NUM; n++) {
    (*test_counter)++;
  }
}

BOOST_AUTO_TEST_CASE(ThreadTest)
{
  uEchoThread* thread = uecho_thread_new();

  int test_counter = 0;
  uecho_thread_setaction(thread, test_theread_func);
  uecho_thread_setuserdata(thread, &test_counter);

  BOOST_REQUIRE_EQUAL(uecho_thread_start(thread), true);
  while (test_counter != THREAD_TEST_LOOP_NUM) {
    uecho_sleep(100);
  }
  BOOST_REQUIRE_EQUAL(test_counter, THREAD_TEST_LOOP_NUM);
  BOOST_REQUIRE_EQUAL(uecho_thread_stop(thread), true);

  uecho_thread_delete(thread);
}
