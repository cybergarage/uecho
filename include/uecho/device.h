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
  uEchoDeviceSuperClassManufacturerCode                      = 0x8A,
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
  uEchoDeviceSuperClassManufacturerCodeSize                      = 3,
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
  uEchoDeviceSuperClassOn      = 0x30,
  uEchoNDeviceSuperClassOff     = 0x31,
  uEchoDeviceSuperClassFaultEncountered   = 0x41,
  uEchoDeviceSuperClassNoFaultEncountered = 0x42
};

bool uecho_device_superclass_addmandatoryproperties(uEchoObject *obj);
bool uecho_device_superclass_setmanufacturercode(uEchoObject *obj, byte *codes);
bool uecho_device_superclass_updatepropertymaps(uEchoObject *obj);
void uecho_device_superclass_clearpropertymapcaches(uEchoObject *obj);

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
