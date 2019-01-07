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

bool uecho_device_addmandatoryproperties(uEchoObject* obj)
{
  if (!obj)
    return false;

  // Operation Status

  uecho_object_setproperty(obj, uEchoDeviceOperatingStatus, uEchoPropertyAttrReadAnno);
  uecho_device_setoperatingstatus(obj, true);

  // Installation Location

  uecho_object_setproperty(obj, uEchoDeviceInstallationLocation, uEchoPropertyAttrReadAnno);
  uecho_device_setinstallationlocation(obj, uEchoDeviceInstallationLocationUnknown);

  // Standard Version Infomation

  uecho_object_setproperty(obj, uEchoDeviceStandardVersion, uEchoPropertyAttrRead);
  uecho_device_setstandardversion(obj, uEchoDeviceDefaultVersionAppendix);

  // Fault Status

  uecho_object_setproperty(obj, uEchoDeviceFaultStatus, uEchoPropertyAttrReadAnno);
  uecho_device_setfaultstatus(obj, false);

  return true;
}

/****************************************
 * uecho_device_setoperatingstatus
 ****************************************/

bool uecho_device_setoperatingstatus(uEchoObject* obj, bool stats)
{
  byte statsByte;

  statsByte = stats ? uEchoDeviceOperatingStatusOn : uEchoDeviceOperatingStatusOff;
  return uecho_object_setpropertydata(obj, uEchoDeviceOperatingStatus, &statsByte, uEchoDeviceOperatingStatusSize);
}

/****************************************
 * uecho_device_isoperatingstatus
 ****************************************/

bool uecho_device_isoperatingstatus(uEchoObject* obj)
{
  byte statsByte;

  if (!uecho_object_getpropertybytedata(obj, uEchoDeviceOperatingStatus, &statsByte))
    return false;

  return (statsByte == uEchoDeviceOperatingStatusOn) ? true : false;
}

/****************************************
 * uecho_device_setinstallationlocation
 ****************************************/

bool uecho_device_setinstallationlocation(uEchoObject* obj, byte locByte)
{
  return uecho_object_setpropertydata(obj, uEchoDeviceInstallationLocation, &locByte, uEchoDeviceInstallationLocationSize);
}

/****************************************
 * uecho_device_getinstallationlocation
 ****************************************/

byte uecho_device_getinstallationlocation(uEchoObject* obj)
{
  byte locByte;

  if (!uecho_object_getpropertybytedata(obj, uEchoDeviceInstallationLocation, &locByte))
    return uEchoDeviceInstallationLocationUnknown;

  return locByte;
}

/****************************************
 * uecho_device_setstandardversion
 ****************************************/

bool uecho_device_setstandardversion(uEchoObject* obj, char ver)
{
  byte verBytes[uEchoDeviceStandardVersionSize];

  verBytes[0] = 0x00;
  verBytes[1] = 0x00;
  verBytes[2] = ver;
  verBytes[3] = 0x00;
  return uecho_object_setpropertydata(obj, uEchoDeviceStandardVersion, verBytes, uEchoDeviceStandardVersionSize);
}

/****************************************
 * uecho_device_getstandardversion
 ****************************************/

char uecho_device_getstandardversion(uEchoObject* obj)
{
  uEchoProperty* prop;
  byte* verBytes;

  prop = uecho_object_getproperty(obj, uEchoDeviceStandardVersion);
  if (!prop)
    return uEchoDeviceVersionUnknown;

  if (uecho_property_getdatasize(prop) != uEchoDeviceStandardVersionSize)
    return uEchoDeviceVersionUnknown;

  verBytes = uecho_property_getdata(prop);
  if (!verBytes)
    return uEchoDeviceVersionUnknown;

  return verBytes[2];
}

/****************************************
 * uecho_device_setfaultstatus
 ****************************************/

bool uecho_device_setfaultstatus(uEchoObject* obj, bool stats)
{
  byte faultByte;

  faultByte = stats ? uEchoDeviceFaultOccurred : uEchoDeviceNoFaultOccurred;
  return uecho_object_setpropertydata(obj, uEchoDeviceFaultStatus, &faultByte, uEchoDeviceFaultStatusSize);
}

/****************************************
 * uecho_device_isfaultstatus
 ****************************************/

bool uecho_device_isfaultstatus(uEchoObject* obj)
{
  byte statsByte;

  if (!uecho_object_getpropertybytedata(obj, uEchoDeviceFaultStatus, &statsByte))
    return false;

  return (statsByte == uEchoDeviceFaultOccurred) ? true : false;
}

/****************************************
 * uecho_device_setmanufacturercode
 ****************************************/

bool uecho_device_setmanufacturercode(uEchoObject* obj, uEchoManufacturerCode code)
{
  return uecho_object_setmanufacturercode(obj, code);
}

/****************************************
 * uecho_device_getmanufacturercode
 ****************************************/

uEchoManufacturerCode uecho_device_getmanufacturercode(uEchoObject* obj)
{
  return uecho_object_getmanufacturercode(obj);
}
