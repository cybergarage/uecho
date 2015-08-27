/************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ************************************************************/

#include "TestDevice.h"

uEchoObject *uecho_test_createtestdevice()
{
  uEchoObject *dev = uecho_device_new();
  uecho_object_setmanufacturercode(dev, UECHO_TEST_MANCODE);
  uecho_object_setcode(dev, UECHO_TEST_OBJECTCODE);
  return dev;
}

uEchoNode *uecho_test_createtestnode() {
  uEchoNode *node = uecho_node_new();
  uEchoObject *dev = uecho_test_createtestdevice();
  uecho_node_addobject(node, dev);
  return node;
}
