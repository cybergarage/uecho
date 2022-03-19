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

#include <uecho/frame/observer.h>

BOOST_AUTO_TEST_CASE(ObjserverListAdd)
{
  uEchoObjectPropertyObserverList* observers = uecho_object_property_observerlist_new();

  BOOST_CHECK_EQUAL(uecho_object_property_observerlist_size(observers), 0);

  for (int n = 0; n < 10; n++) {
    BOOST_CHECK(uecho_object_property_observerlist_add(observers, uecho_object_property_observer_new()));
    BOOST_CHECK_EQUAL(uecho_object_property_observerlist_size(observers), (n + 1));
  }

  uecho_object_property_observerlist_clear(observers);
  BOOST_CHECK_EQUAL(uecho_object_property_observerlist_size(observers), 0);

  uecho_object_property_observerlist_delete(observers);
}

BOOST_AUTO_TEST_CASE(ObjectPropetyObserverManagerNew)
{
  uEchoObjectPropertyObserverManager* mgr = uecho_object_property_observer_manager_new();

  uecho_object_property_observer_manager_delete(mgr);
}
