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

#include <uecho/device.h>

BOOST_AUTO_TEST_CASE(DeviceSuperClassProperties)
{
  uEchoObject *obj = uecho_device_new();
  
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceSuperClassAnnoPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceSuperClassSetPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceSuperClassGetPropertyMap));
  
  uecho_object_delete(obj);
}
