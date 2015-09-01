/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/device.h>

/****************************************
* uecho_device_new
****************************************/

uEchoObject *uecho_device_new(void)
{
  uEchoObject *obj;

  obj = uecho_object_new();
    
  if (!obj)
    return NULL;
  
  uecho_device_addmandatoryproperties(obj);

  return obj;
}
