/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

#include <uecho/util/thread.h>
#include <uecho/util/timer.h>

const int THREAD_TEST_LOOP_NUM = 10;

void TestThereadFunc(uEchoThread* thread)
{
  int* testCounter = (int*)uecho_thread_getuserdata(thread);
  for (int n = 0; n < THREAD_TEST_LOOP_NUM; n++) {
    (*testCounter)++;
  }
}

BOOST_AUTO_TEST_CASE(ThreadTest)
{
  uEchoThread* thread = uecho_thread_new();

  int testCounter = 0;
  uecho_thread_setaction(thread, TestThereadFunc);
  uecho_thread_setuserdata(thread, &testCounter);

  BOOST_CHECK_EQUAL(uecho_thread_start(thread), true);
  while (testCounter != THREAD_TEST_LOOP_NUM) {
    uecho_sleep(100);
  }
  BOOST_CHECK_EQUAL(testCounter, THREAD_TEST_LOOP_NUM);
  BOOST_CHECK_EQUAL(uecho_thread_stop(thread), true);

  uecho_thread_delete(thread);
}
