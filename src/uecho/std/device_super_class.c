/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/device.h>

/****************************************
 * uecho_device_addmandatoryproperties
 ****************************************/

bool uecho_device_addmandatoryproperties(uEchoObject *obj)
{
  byte manufactureCode[] = {0, 0, 0};
  byte zeroPropMap[] = {0};
  
  if (!obj)
    return false;
  
  if (!uecho_object_addproperty(obj, uEchoObjectGetPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  
  return true;
}

/****************************************
 * uecho_device_setmanufacturercode
 ****************************************/

bool uecho_device_setmanufacturercode(uEchoObject *obj, byte *codes)
{
  return uecho_object_setmanufacturercode(obj, codes);
}
