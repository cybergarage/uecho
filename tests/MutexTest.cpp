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

#include <uecho/util/mutex.h>

BOOST_AUTO_TEST_CASE(MutexText)
{
  uEchoMutex *mutex = uecho_mutex_new();
  BOOST_CHECK_EQUAL(uecho_mutex_lock(mutex), true);
  BOOST_CHECK_EQUAL(uecho_mutex_unlock(mutex), true);
  uecho_mutex_delete(mutex);
}
Ω
