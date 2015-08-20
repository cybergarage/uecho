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
  byte propData[32];
  
  if (!obj)
    return false;
  
  // Operation Status
  
  propData[0] = uEchoDeviceOperatingStatusOn;
  uecho_object_addproperty(obj, uEchoDeviceOperatingStatus, uEchoPropertyAttrRead, propData, uEchoDeviceOperatingStatusSize);
  
  // Installation Location
  
  propData[0] = uEchoDeviceInstallationLocationUnknown;
  uecho_object_addproperty(obj, uEchoDeviceInstallationLocation, uEchoPropertyAttrRead, propData, uEchoDeviceInstallationLocationSize);
  
  return true;
}

/****************************************
 * uecho_device_setoperatingstatus
 ****************************************/

bool uecho_device_setoperatingstatus(uEchoObject *obj, bool stats)
{
  byte statsByte;
  
  statsByte = stats ? uEchoDeviceOperatingStatusOn : uEchoDeviceOperatingStatusOff;
  return uecho_object_updatepropertydata(obj, uEchoDeviceOperatingStatus, &statsByte, uEchoDeviceOperatingStatusSize);
}

/****************************************
 * uecho_device_setinstallationlocation
 ****************************************/

bool uecho_device_setinstallationlocation(uEchoObject *obj, byte locByte)
{
  return uecho_object_updatepropertydata(obj, uEchoDeviceInstallationLocation, &locByte, uEchoDeviceInstallationLocationSize);
}

/****************************************
 * uecho_device_setstandardversion
 ****************************************/

bool uecho_device_setstandardversion(uEchoObject *obj, char ver)
{
  return true;
}

/****************************************
 * uecho_device_setfaultstatus
 ****************************************/

bool uecho_device_setfaultstatus(uEchoObject *obj, bool stats)
{
  return true;
}

/****************************************
 * uecho_device_setmanufacturercode
 ****************************************/

bool uecho_device_setmanufacturercode(uEchoObject *obj, byte *codes)
{
  return uecho_object_setmanufacturercode(obj, codes);
}
