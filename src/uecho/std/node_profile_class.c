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

#include <uecho/const.h>
#include <uecho/profile.h>

/****************************************
* uecho_property_new
****************************************/

uEchoObject *uecho_nodeprofileclass_new(void)
{
  uEchoObject *obj;
  
  obj = uecho_object_new();
  if (!obj)
    return NULL;
  
  uecho_object_setcode(obj, uEchoNodeProfileObject);
  uecho_nodeprofileclass_addmandatoryproperties(obj);
  
  return obj;
}

/****************************************
 * uecho_nodeprofileclass_addmandatoryproperties
 ****************************************/

bool uecho_nodeprofileclass_addmandatoryproperties(uEchoObject *obj)
{
  byte propData[32];

  // Operation Status
  
  propData[0] = uEchoNodeProfileClassBooting;
  uecho_object_addproperty(obj, uEchoNodeProfileClassOperatingStatus, uEchoPropertyAttrRead, propData, uEchoNodeProfileClassOperatingStatusLen);
  
  // Version Information
  
  propData[0] = uEchoMajorVersion;
  propData[1] = uEchoMinorVersion;
  propData[2] = uEchoSpecifiedMessageFormat;
  propData[3] = 0x00;
  uecho_object_addproperty(obj, uEchoNodeProfileClassVersionInformation, uEchoPropertyAttrRead, propData, uEchoNodeProfileClassVersionInformationLen);
  
  return true;
}

/****************************************
 * uecho_nodeprofileclass_setoperatingstatus
 ****************************************/

bool uecho_nodeprofileclass_setoperatingstatus(uEchoObject *obj, bool stats)
{
  byte statsByte;
  
  statsByte = stats ? uEchoNodeProfileClassBooting : uEchoNodeProfileClassNotBooting;
  return uecho_object_updatepropertydata(obj, uEchoNodeProfileClassOperatingStatus, &statsByte, uEchoNodeProfileClassOperatingStatusLen);
}

/****************************************
 * uecho_nodeprofileclass_setid
 ****************************************/

bool uecho_nodeprofileclass_setid(uEchoObject *obj, byte *manCode, byte *uniqId)
{
  byte propData[uEchoNodeProfileClassIdentificationNumberLen];
  byte *prop;
  
  propData[0] = uEchoLowerCommunicationLayerProtocolType;

  prop = propData + 1;
  memcpy(prop, manCode, uEchoNodeProfileClassIdentificationManufacturerCodeLen);
  
  prop += uEchoNodeProfileClassIdentificationManufacturerCodeLen;
  memcpy(prop, uniqId, uEchoNodeProfileClassIdentificationUniqueIdLen);
  
  return uecho_object_addproperty(obj, uEchoNodeProfileClassOperatingStatus, uEchoPropertyAttrRead, propData, uEchoNodeProfileClassIdentificationNumberLen);
}
