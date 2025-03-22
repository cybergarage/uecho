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

#include <uecho/_controller.h>
#include <uecho/device.h>

#include "TestDevice.h"

BOOST_AUTO_TEST_CASE(DeviceSuperClassMandatoryProperties)
{
  uEchoObject* obj = uecho_device_new();

  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceAnnoPropertyMap));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceSetPropertyMap));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceGetPropertyMap));

  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceOperatingStatus));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceInstallationLocation));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceStandardVersion));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceFaultStatus));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoDeviceManufacturerCode));

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(DeviceDefaultProperties)
{
  uEchoObject* obj = uecho_device_new();

  BOOST_REQUIRE_EQUAL(uecho_device_isoperatingstatus(obj), true);
  BOOST_REQUIRE_EQUAL(uecho_device_isfaultstatus(obj), false);
  BOOST_REQUIRE_EQUAL(uecho_device_getinstallationlocation(obj), (byte)uEchoDeviceInstallationLocationUnknown);
  BOOST_REQUIRE_EQUAL(uecho_device_getstandardversion(obj), (char)uEchoDeviceVersionAppendixDefault);
  BOOST_REQUIRE_EQUAL(uecho_device_getmanufacturercode(obj), (uEchoManufactureCode)uEchoManufactureCodeDefault);

  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(DeviceRequest)
{
  uEchoMessage* msg;
  byte propByte;

  // Create Controller (Disable UDP Server)

  uEchoController* ctrl = uecho_controller_new();
  BOOST_REQUIRE(uecho_controller_start(ctrl));
  BOOST_REQUIRE(uecho_controller_isrunning(ctrl));

  // Add Device

  uEchoNode* node = uecho_controller_getlocalnode(ctrl);
  uEchoObject* dev = uecho_test_createtestdevice();
  BOOST_REQUIRE(uecho_node_addobject(node, dev));

  // Start Controller and Device

  BOOST_REQUIRE(uecho_controller_start(ctrl));
  BOOST_REQUIRE(uecho_controller_isrunning(ctrl));

  // Search (NotificationRequest instead of ReadRequest)

  BOOST_REQUIRE(uecho_controller_searchwithesv(ctrl, uEchoEsvNotificationRequest));

  // Find device

  uEchoObject* foundObj = uecho_controller_getobjectbycodewithwait(ctrl, UECHO_TEST_OBJECTCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_REQUIRE(foundObj);
  if (!foundObj)
    return;

  uEchoNode* foundNode = uecho_object_getparentnode(foundObj);
  BOOST_REQUIRE(foundNode);
  if (!foundNode)
    return;

  // Send Message (ReadRequest)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));
  BOOST_REQUIRE(uecho_controller_sendmessage(ctrl, foundNode, msg));
  uecho_message_delete(msg);

  // Check Property (Defult:ON)

  uEchoProperty* foundProp = uecho_object_getpropertywait(foundObj, UECHO_TEST_PROPERTY_SWITCHCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_REQUIRE(foundProp);
  for (int n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(UECHO_TEST_RESPONSE_WAIT_MAX_MTIME / uEchoWaitRetryCount);
    if (!uecho_property_getbytedata(foundProp, &propByte))
      continue;
    if (propByte == UECHO_TEST_PROPERTY_SWITCH_DEFAULT)
      break;
  }
  BOOST_REQUIRE(uecho_property_getbytedata(foundProp, &propByte));
  BOOST_REQUIRE_EQUAL(propByte, UECHO_TEST_PROPERTY_SWITCH_DEFAULT);

  // Send Message (WriteRequest:OFF)

  propByte = UECHO_TEST_PROPERTY_SWITCH_OFF;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, &propByte, 1));
  BOOST_REQUIRE(uecho_controller_sendmessage(ctrl, foundNode, msg));
  uecho_message_delete(msg);

  // Send Message (ReadRequest)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));
  BOOST_REQUIRE(uecho_controller_sendmessage(ctrl, foundNode, msg));
  uecho_message_delete(msg);

  // Check Property Update (OFF)

  bool propChanged = false;
  for (int n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(UECHO_TEST_RESPONSE_WAIT_MAX_MTIME / uEchoWaitRetryCount);
    if (!uecho_property_getbytedata(foundProp, &propByte))
      continue;
    if (propByte == UECHO_TEST_PROPERTY_SWITCH_OFF) {
      propChanged = true;
      break;
    }
  }
  BOOST_REQUIRE(propChanged);

  // Send Message (WriteReadRequest:ON)

  propByte = UECHO_TEST_PROPERTY_SWITCH_ON;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setpropertyset(msg, UECHO_TEST_PROPERTY_SWITCHCODE, &propByte, 1));
  BOOST_REQUIRE(uecho_message_setpropertyget(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));
  BOOST_REQUIRE(uecho_controller_sendmessage(ctrl, foundNode, msg));
  uecho_message_delete(msg);

  // Check Property Update (ON)

  propChanged = false;
  for (int n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(UECHO_TEST_RESPONSE_WAIT_MAX_MTIME / uEchoWaitRetryCount);
    if (!uecho_property_getbytedata(foundProp, &propByte))
      continue;
    if (propByte == UECHO_TEST_PROPERTY_SWITCH_ON) {
      propChanged = true;
      break;
    }
  }
  BOOST_REQUIRE(propChanged);

  // Teminate

  BOOST_REQUIRE(uecho_controller_stop(ctrl));
  uecho_controller_delete(ctrl);
}
