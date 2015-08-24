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

#include <uecho/object.h>

BOOST_AUTO_TEST_CASE(ObjectPropetyObserverManagerNew)
{
  uEchoObjectPropertyObserverManager *mgr = uecho_object_property_observer_manager_new();
  
  uecho_object_property_observer_manager_delete(mgr);
}
