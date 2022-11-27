/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/device.h>

/****************************************
 * uecho_device_new
 ****************************************/

uEchoObject* uecho_device_new(void)
{
  uEchoObject* obj;

  obj = uecho_object_new();

  if (!obj)
    return NULL;

  if (!uecho_device_addmandatoryproperties(obj)) {
    uecho_object_delete(obj);
    return NULL;
  }

  return obj;
}
