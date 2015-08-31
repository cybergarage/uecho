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

#include <uecho/class_internal.h>

const int uEchoTestClassCodeMax = uEchoClassCodeMax % 100;

BOOST_AUTO_TEST_CASE(ClassListAdd)
{
  uEchoClassList *clsses = uecho_classlist_new();

  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), 0);
  
  for (size_t n=uEchoClassCodeMin; n<=uEchoTestClassCodeMax; n++) {
    uEchoClass *cls = uecho_class_new();
    uecho_class_setcode(cls, (uEchoClassCode)n);
    BOOST_CHECK(uecho_classlist_add(clsses, cls));
  }

  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), (uEchoTestClassCodeMax - uEchoClassCodeMin + 1));
  
  for (size_t n=uEchoClassCodeMin; n<=uEchoTestClassCodeMax; n++) {
    uEchoClass *cls = uecho_classlist_getbycode(clsses, (uEchoClassCode)n);
    BOOST_CHECK(cls);
    BOOST_CHECK_EQUAL(uecho_class_getcode(cls), n);
  }

  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), (uEchoTestClassCodeMax - uEchoClassCodeMin + 1));

  BOOST_CHECK(uecho_classlist_clear(clsses));
  
  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), 0);

  uecho_classlist_delete(clsses);
}

BOOST_AUTO_TEST_CASE(ClassListSet)
{
  uEchoClassList *clsses = uecho_classlist_new();
  
  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), 0);
  
  for (size_t n=uEchoClassCodeMin; n<=uEchoTestClassCodeMax; n++) {
    BOOST_CHECK(uecho_classlist_set(clsses, (uEchoClassCode)n));
  }
  
  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), (uEchoTestClassCodeMax - uEchoClassCodeMin + 1));

  for (size_t n=uEchoClassCodeMin; n<=uEchoTestClassCodeMax; n++) {
    uEchoClass *cls = uecho_classlist_getbycode(clsses, (uEchoClassCode)n);
    BOOST_CHECK(cls);
    BOOST_CHECK_EQUAL(uecho_class_getcode(cls), n);
  }
  
  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), (uEchoTestClassCodeMax - uEchoClassCodeMin + 1));
  
  BOOST_CHECK(uecho_classlist_clear(clsses));
  
  BOOST_CHECK_EQUAL(uecho_classlist_size(clsses), 0);
  
  uecho_classlist_delete(clsses);
}
