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
  uEchoObject *dev = uecho_device_new();
  return dev;
}
