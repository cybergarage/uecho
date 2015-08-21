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

BOOST_AUTO_TEST_CASE(DeviceSuperClassMandatoryProperties)
{
  uEchoObject *obj = uecho_device_new();
  
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceAnnoPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceSetPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceGetPropertyMap));

  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceOperatingStatus));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceInstallationLocation));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceStandardVersion));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceFaultStatus));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoDeviceManufacturerCode));
  
  uecho_object_delete(obj);
}
