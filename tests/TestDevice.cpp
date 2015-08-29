/************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ************************************************************/

#include "TestDevice.h"

uEchoObject *uecho_test_createtestdevice()
{
  uEchoObject *obj = uecho_device_new();
  
  uecho_object_setmanufacturercode(obj, UECHO_TEST_MANCODE);
  uecho_object_setcode(obj, UECHO_TEST_OBJECTCODE);

  byte prop[32];

  // Operation status property
  uecho_object_setproperty(obj, UECHO_TEST_PROPERTY_SWITCHCODE, uEchoPropertyAttrReadWrite);
  prop[0] = UECHO_TEST_PROPERTY_SWITCH_ON;
  uecho_object_setpropertydata(obj, UECHO_TEST_PROPERTY_SWITCHCODE, prop, 1);
  
  return obj;
}

uEchoNode *uecho_test_createtestnode() {
  uEchoNode *node = uecho_node_new();
  uEchoObject *dev = uecho_test_createtestdevice();
  uecho_node_addobject(node, dev);
  return node;
}
