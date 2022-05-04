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

#include <uecho/_object.h>
#include <uecho/device.h>
#include <uecho/std/database.h>

/****************************************
 * uecho_device_addmandatoryproperties
 ****************************************/

bool uecho_device_addmandatoryproperties(uEchoObject* obj)
{
  uEchoDatabase* db;
  uEchoObject *super_obj;

  if (!obj)
    return false;

  db = uecho_standard_getdatabase();
  if (!db)
    return false;

  super_obj = uecho_database_getobject(db, 0x00, 0x00);
  if (!super_obj)
    return false;

  if (!uecho_object_copyobjectproperties(obj, super_obj))
    return false;
  
  if (!uecho_device_setoperatingstatus(obj, true))
    return false;
  
  if (!uecho_device_setstandardversion(obj, uEchoDeviceDefaultVersionAppendix))
    return false;
  
  return true;
}

/****************************************
 * uecho_device_setoperatingstatus
 ****************************************/

bool uecho_device_setoperatingstatus(uEchoObject* obj, bool stats)
{
  byte stats_byte;

  stats_byte = stats ? uEchoDeviceOperatingStatusOn : uEchoDeviceOperatingStatusOff;
  return uecho_object_setpropertydata(obj, uEchoDeviceOperatingStatus, &stats_byte, uEchoDeviceOperatingStatusSize);
}

/****************************************
 * uecho_device_isoperatingstatus
 ****************************************/

bool uecho_device_isoperatingstatus(uEchoObject* obj)
{
  byte stats_byte;

  if (!uecho_object_getpropertybytedata(obj, uEchoDeviceOperatingStatus, &stats_byte))
    return false;

  return (stats_byte == uEchoDeviceOperatingStatusOn) ? true : false;
}

/****************************************
 * uecho_device_setinstallationlocation
 ****************************************/

bool uecho_device_setinstallationlocation(uEchoObject* obj, byte loc_byte)
{
  return uecho_object_setpropertydata(obj, uEchoDeviceInstallationLocation, &loc_byte, uEchoDeviceInstallationLocationSize);
}

/****************************************
 * uecho_device_getinstallationlocation
 ****************************************/

byte uecho_device_getinstallationlocation(uEchoObject* obj)
{
  byte loc_byte;

  if (!uecho_object_getpropertybytedata(obj, uEchoDeviceInstallationLocation, &loc_byte))
    return uEchoDeviceInstallationLocationUnknown;

  return loc_byte;
}

/****************************************
 * uecho_device_setstandardversion
 ****************************************/

bool uecho_device_setstandardversion(uEchoObject* obj, char ver)
{
  byte ver_bytes[uEchoDeviceStandardVersionSize];

  ver_bytes[0] = 0x00;
  ver_bytes[1] = 0x00;
  ver_bytes[2] = ver;
  ver_bytes[3] = 0x00;
  return uecho_object_setpropertydata(obj, uEchoDeviceStandardVersion, ver_bytes, uEchoDeviceStandardVersionSize);
}

/****************************************
 * uecho_device_getstandardversion
 ****************************************/

char uecho_device_getstandardversion(uEchoObject* obj)
{
  uEchoProperty* prop;
  byte* ver_bytes;

  prop = uecho_object_getproperty(obj, uEchoDeviceStandardVersion);
  if (!prop)
    return uEchoDeviceVersionUnknown;

  if (uecho_property_getdatasize(prop) != uEchoDeviceStandardVersionSize)
    return uEchoDeviceVersionUnknown;

  ver_bytes = uecho_property_getdata(prop);
  if (!ver_bytes)
    return uEchoDeviceVersionUnknown;

  return ver_bytes[2];
}

/****************************************
 * uecho_device_setfaultstatus
 ****************************************/

bool uecho_device_setfaultstatus(uEchoObject* obj, bool stats)
{
  byte fault_byte;

  fault_byte = stats ? uEchoDeviceFaultOccurred : uEchoDeviceNoFaultOccurred;
  return uecho_object_setpropertydata(obj, uEchoDeviceFaultStatus, &fault_byte, uEchoDeviceFaultStatusSize);
}

/****************************************
 * uecho_device_isfaultstatus
 ****************************************/

bool uecho_device_isfaultstatus(uEchoObject* obj)
{
  byte stats_byte;

  if (!uecho_object_getpropertybytedata(obj, uEchoDeviceFaultStatus, &stats_byte))
    return false;

  return (stats_byte == uEchoDeviceFaultOccurred) ? true : false;
}

/****************************************
 * uecho_device_setmanufacturercode
 ****************************************/

bool uecho_device_setmanufacturercode(uEchoObject* obj, uEchoManufactureCode code)
{
  return uecho_object_setmanufacturercode(obj, code);
}

/****************************************
 * uecho_device_getmanufacturercode
 ****************************************/

uEchoManufactureCode uecho_device_getmanufacturercode(uEchoObject* obj)
{
  return uecho_object_getmanufacturercode(obj);
}
