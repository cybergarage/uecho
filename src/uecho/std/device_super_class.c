/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/device.h>

/****************************************
 * uecho_device_superclass_addmandatoryproperties
 ****************************************/

bool uecho_device_superclass_addmandatoryproperties(uEchoObject *obj)
{
  if (!obj)
    return false;
  
  // Property map properties
  
  uecho_object_superclass_addmandatoryproperties(obj);
  
  return true;
}
