/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
  uEchoDeviceSuperClassOperatingStatus                       = 0x80,
  uEchoDeviceSuperClassInstallationLocation                  = 0x81,
  uEchoDeviceSuperClassVersionInformation                    = 0x82,
  uEchoDeviceSuperClassIdentificationNumber                  = 0x83,
  uEchoDeviceSuperClassMeasuredInstantaneousPowerConsumption = 0x84,
  uEchoDeviceSuperClassMeasuredCumulativePowerConsumption    = 0x85,
  uEchoDeviceSuperClassManufacturerFaultCode                 = 0x86,
  uEchoDeviceSuperClassCurrentLimitSetting                   = 0x87,
  uEchoDeviceSuperClassFaultStatus                           = 0x88,
  uEchoDeviceSuperClassFaultDescription                      = 0x89,
  uEchoDeviceSuperClassManufacturerCode                      = uEchoObjectSuperClassManufacturerCode,
  uEchoDeviceSuperClassBusinessFacilityCode                  = 0x8B,
  uEchoDeviceSuperClassProductCode                           = 0x8C,
  uEchoDeviceSuperClassProductionNumber                      = 0x8D,
  uEchoDeviceSuperClassProductionDate                        = 0x8E,
  uEchoDeviceSuperClassPowerSavingOperationSetting           = 0x8F,
  uEchoDeviceSuperClassRemoteControlSetting                  = 0x93,
  uEchoDeviceSuperClassCurrentTimeSetting                    = 0x97,
  uEchoDeviceSuperClassCurrentDateSetting                    = 0x98,
  uEchoDeviceSuperClassPowerLimitSetting                     = 0x99,
  uEchoDeviceSuperClassCumulativeOperatingTime               = 0x9A,
  uEchoDeviceSuperClassAnnoPropertyMap                       = uEchoObjectSuperClassAnnoPropertyMap,
  uEchoDeviceSuperClassSetPropertyMap                        = uEchoObjectSuperClassSetPropertyMap,
  uEchoDeviceSuperClassGetPropertyMap                        = uEchoObjectSuperClassGetPropertyMap,
} uEchoDeviceSuperClassEPC;

typedef enum {
  uEchoDeviceSuperClassOperatingStatusSize                       = 1,
  uEchoDeviceSuperClassInstallationLocationSize                  = 1,
  uEchoDeviceSuperClassVersionInformationSize                    = 4,
  uEchoDeviceSuperClassIdentificationNumberSize                  = 9,
  uEchoDeviceSuperClassMeasuredInstantaneousPowerConsumptionSize = 2,
  uEchoDeviceSuperClassMeasuredCumulativePowerConsumptionSize    = 4,
  uEchoDeviceSuperClassManufacturerFaultCodeSize                 = 255,
  uEchoDeviceSuperClassCurrentLimitSettingSize                   = 1,
  uEchoDeviceSuperClassFaultStatusSize                           = 1,
  uEchoDeviceSuperClassFaultDescriptionSize                      = 2,
  uEchoDeviceSuperClassManufacturerCodeSize                      = uEchoObjectSuperClassManufacturerCodeLen,
  uEchoDeviceSuperClassBusinessFacilityCodeSize                  = 3,
  uEchoDeviceSuperClassProductCodeSize                           = 12,
  uEchoDeviceSuperClassProductionNumberSize                      = 12,
  uEchoDeviceSuperClassProductionDateSize                        = 4,
  uEchoDeviceSuperClassPowerSavingOperationSettingSize           = 1,
  uEchoDeviceSuperClassRemoteControlSettingSize                  = 1,
  uEchoDeviceSuperClassCurrentTimeSettingSize                    = 2,
  uEchoDeviceSuperClassCurrentDateSettingSize                    = 4,
  uEchoDeviceSuperClassPowerLimitSettingSize                     = 2,
  uEchoDeviceSuperClassCumulativeOperatingTimeSize               = 5,
  uEchoDeviceSuperClassAnnoPropertyMapSize                       = uEchoObjectSuperClassAnnoPropertyMapMaxLen,
  uEchoDeviceSuperClassSetPropertyMapSize                        = uEchoObjectSuperClassSetPropertyMapMaxLen,
  uEchoDeviceSuperClassGetPropertyMapSize                        = uEchoObjectSuperClassGetPropertyMapMaxLen,
} uEchoDeviceSuperClassEPCSize;

enum {
  uEchoDeviceSuperClassOn                          = 0x30,
  uEchoDeviceSuperClassOff                         = 0x31,
  uEchoDeviceSuperClassVersionAppendixG            = 'G',
  uEchoDeviceSuperClassFaultOccurred               = 0x41,
  uEchoDeviceSuperClassNoFaultOccurred             = 0x42,
  uEchoDeviceSuperClassInstallationLocationUnknown = 0x00,
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
