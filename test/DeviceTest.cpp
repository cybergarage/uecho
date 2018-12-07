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

#include <uecho/controller_internal.h>
#include "TestDevice.h"
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

BOOST_AUTO_TEST_CASE(DeviceDefaultProperties)
{
  uEchoObject *obj = uecho_device_new();
  
  BOOST_CHECK_EQUAL(uecho_device_isoperatingstatus(obj), true);
  BOOST_CHECK_EQUAL(uecho_device_isfaultstatus(obj), false);
  BOOST_CHECK_EQUAL(uecho_device_getinstallationlocation(obj), (byte)uEchoDeviceInstallationLocationUnknown);
  BOOST_CHECK_EQUAL(uecho_device_getstandardversion(obj), (char)uEchoDeviceDefaultVersionAppendix);
  BOOST_CHECK_EQUAL(uecho_device_getmanufacturercode(obj), (uEchoManufacturerCode)uEchoManufactureCodeDefault);
  
  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(DeviceRequest)
{
  uEchoMessage *msg;
  byte propByte;
  
  // Create Controller (Disable UDP Server)
  
  uEchoController *ctrl = uecho_controller_new();
  uecho_controller_disableudpserver(ctrl);
  BOOST_CHECK(uecho_controller_start(ctrl));
  BOOST_CHECK(uecho_controller_isrunning(ctrl));
  
  // Start Device
  
  uEchoNode *node = uecho_test_createtestnode();
  BOOST_CHECK(uecho_node_start(node));
  BOOST_CHECK(uecho_node_isrunning(node));
  
  // Search (NotificationRequest instead of ReadRequest)
  
  BOOST_CHECK(uecho_controller_searchallobjectswithesv(ctrl, uEchoEsvNotificationRequest));
  
  // Find device

  uEchoObject *foundObj = uecho_controller_getobjectbycodewithwait(ctrl, UECHO_TEST_OBJECTCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_CHECK(foundObj);
  
  // Send Message (NotificationRequest instead of ReadRequest)
   
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvNotificationRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 0, NULL));
  BOOST_CHECK(uecho_controller_sendmessage(ctrl, foundObj, msg));
  uecho_message_delete(msg);
  
  // Check Property (Defult:ON)
   
  uEchoProperty *foundProp = uecho_object_getpropertywait(foundObj, UECHO_TEST_PROPERTY_SWITCHCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_CHECK(foundProp);
  for (int n=0; n<uEchoWaitRetryCount; n++) {
    uecho_sleep(UECHO_TEST_RESPONSE_WAIT_MAX_MTIME / uEchoWaitRetryCount);
    if (!uecho_property_getbytedata(foundProp, &propByte))
      continue;
    if (propByte == UECHO_TEST_PROPERTY_SWITCH_DEFAULT)
      break;
  }
  BOOST_CHECK(uecho_property_getbytedata(foundProp, &propByte));
  BOOST_CHECK_EQUAL(propByte, UECHO_TEST_PROPERTY_SWITCH_DEFAULT);
  
  // Send Message (WriteRequest:OFF)
  
  propByte = UECHO_TEST_PROPERTY_SWITCH_OFF;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 1, &propByte));
  BOOST_CHECK(uecho_controller_sendmessage(ctrl, foundObj, msg));
  uecho_message_delete(msg);
  
  
  // Send Message (NotificationRequest instead of ReadRequest)
  
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvNotificationRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 0, NULL));
  BOOST_CHECK(uecho_controller_sendmessage(ctrl, foundObj, msg));
  uecho_message_delete(msg);
  
  // Check Property Update (OFF)
  
  bool propChanged = false;
  for (int n=0; n<uEchoWaitRetryCount; n++) {
    uecho_sleep(UECHO_TEST_RESPONSE_WAIT_MAX_MTIME / uEchoWaitRetryCount);
    if (!uecho_property_getbytedata(foundProp, &propByte))
      continue;
    if (propByte == UECHO_TEST_PROPERTY_SWITCH_OFF) {
      propChanged = true;
      break;
    }
  }
  BOOST_CHECK(propChanged);
  
  // Send Message (WriteRequest:ON)
  
  propByte = UECHO_TEST_PROPERTY_SWITCH_ON;
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 1, &propByte));
  BOOST_CHECK(uecho_controller_sendmessage(ctrl, foundObj, msg));
  uecho_message_delete(msg);
  
  // Send Message (NotificationRequest instead of ReadRequest)
  
  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvNotificationRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 0, NULL));
  BOOST_CHECK(uecho_controller_sendmessage(ctrl, foundObj, msg));
  uecho_message_delete(msg);

  // Check Property Update (ON)
  
  propChanged = false;
  for (int n=0; n<uEchoWaitRetryCount; n++) {
    uecho_sleep(UECHO_TEST_RESPONSE_WAIT_MAX_MTIME / uEchoWaitRetryCount);
    if (!uecho_property_getbytedata(foundProp, &propByte))
      continue;
    if (propByte == UECHO_TEST_PROPERTY_SWITCH_ON) {
      propChanged = true;
      break;
    }
  }
  BOOST_CHECK(propChanged);
  
  // Teminate
  
  BOOST_CHECK(uecho_controller_stop(ctrl));
  uecho_controller_delete(ctrl);
  
  BOOST_CHECK(uecho_node_stop(node));
  uecho_node_delete(node);
}
