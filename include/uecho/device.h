/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_DEVICE_H_
#define _UECHO_DEVICE_H_

#include <uecho/object.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Device
 ****************************************/

uEchoObject *uecho_device_new(void);
  
/****************************************
 * Device Object Super Class
 ****************************************/
  
typedef enum {
  uEchoDeviceOperatingStatus                       = 0x80,
  uEchoDeviceInstallationLocation                  = 0x81,
  uEchoDeviceStandardVersion                       = 0x82,
  uEchoDeviceIdentificationNumber                  = 0x83,
  uEchoDeviceMeasuredInstantaneousPowerConsumption = 0x84,
  uEchoDeviceMeasuredCumulativePowerConsumption    = 0x85,
  uEchoDeviceManufacturerFaultCode                 = 0x86,
  uEchoDeviceCurrentLimitSetting                   = 0x87,
  uEchoDeviceFaultStatus                           = 0x88,
  uEchoDeviceFaultDescription                      = 0x89,
  uEchoDeviceManufacturerCode                      = uEchoObjectManufacturerCode,
  uEchoDeviceBusinessFacilityCode                  = 0x8B,
  uEchoDeviceProductCode                           = 0x8C,
  uEchoDeviceProductionNumber                      = 0x8D,
  uEchoDeviceProductionDate                        = 0x8E,
  uEchoDevicePowerSavingOperationSetting           = 0x8F,
  uEchoDeviceRemoteControlSetting                  = 0x93,
  uEchoDeviceCurrentTimeSetting                    = 0x97,
  uEchoDeviceCurrentDateSetting                    = 0x98,
  uEchoDevicePowerLimitSetting                     = 0x99,
  uEchoDeviceCumulativeOperatingTime               = 0x9A,
  uEchoDeviceAnnoPropertyMap                       = uEchoObjectAnnoPropertyMap,
  uEchoDeviceSetPropertyMap                        = uEchoObjectSetPropertyMap,
  uEchoDeviceGetPropertyMap                        = uEchoObjectGetPropertyMap,
} uEchoDeviceEPC;

typedef enum {
  uEchoDeviceOperatingStatusSize                       = 1,
  uEchoDeviceInstallationLocationSize                  = 1,
  uEchoDeviceStandardVersionSize                       = 4,
  uEchoDeviceIdentificationNumberSize                  = 9,
  uEchoDeviceMeasuredInstantaneousPowerConsumptionSize = 2,
  uEchoDeviceMeasuredCumulativePowerConsumptionSize    = 4,
  uEchoDeviceManufacturerFaultCodeSize                 = 255,
  uEchoDeviceCurrentLimitSettingSize                   = 1,
  uEchoDeviceFaultStatusSize                           = 1,
  uEchoDeviceFaultDescriptionSize                      = 2,
  uEchoDeviceManufacturerCodeSize                      = uEchoObjectManufacturerCodeLen,
  uEchoDeviceBusinessFacilityCodeSize                  = 3,
  uEchoDeviceProductCodeSize                           = 12,
  uEchoDeviceProductionNumberSize                      = 12,
  uEchoDeviceProductionDateSize                        = 4,
  uEchoDevicePowerSavingOperationSettingSize           = 1,
  uEchoDeviceRemoteControlSettingSize                  = 1,
  uEchoDeviceCurrentTimeSettingSize                    = 2,
  uEchoDeviceCurrentDateSettingSize                    = 4,
  uEchoDevicePowerLimitSettingSize                     = 2,
  uEchoDeviceCumulativeOperatingTimeSize               = 5,
  uEchoDeviceAnnoPropertyMapSize                       = uEchoObjectAnnoPropertyMapMaxLen,
  uEchoDeviceSetPropertyMapSize                        = uEchoObjectSetPropertyMapMaxLen,
  uEchoDeviceGetPropertyMapSize                        = uEchoObjectGetPropertyMapMaxLen,
} uEchoDevicenEPCSize;

enum {
  uEchoDeviceOperatingStatusOn           = 0x30,
  uEchoDeviceOperatingStatusOff          = 0x31,
  uEchoDeviceVersionAppendixA            = 'A',
  uEchoDeviceVersionAppendixB            = 'B',
  uEchoDeviceVersionAppendixC            = 'C',
  uEchoDeviceVersionAppendixD            = 'D',
  uEchoDeviceVersionAppendixE            = 'E',
  uEchoDeviceVersionAppendixF            = 'F',
  uEchoDeviceVersionAppendixG            = 'G',
  uEchoDeviceFaultOccurred               = 0x41,
  uEchoDeviceNoFaultOccurred             = 0x42,
  uEchoDeviceInstallationLocationUnknown = 0x00,
};

bool uecho_device_addmandatoryproperties(uEchoObject *obj);

bool uecho_device_setoperatingstatus(uEchoObject *obj, bool stats);
bool uecho_device_setinstallationlocation(uEchoObject *obj, byte location);
bool uecho_device_setstandardversion(uEchoObject *obj, char ver);
bool uecho_device_setfaultstatus(uEchoObject *obj, bool stats);
bool uecho_device_setmanufacturercode(uEchoObject *obj, byte *codes);

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
