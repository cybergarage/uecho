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
/*
 uEchoProfileNodeProfileClassOperatingStatus           = 0x80,
 uEchoProfileNodeProfileClassVersionInformation        = 0x82,
 uEchoProfileNodeProfileClassIdentificationNumber      = 0x83,
 uEchoProfileNodeProfileClassFaultContent              = 0x89,
 uEchoProfileNodeProfileClassUniqueIdentifierData      = 0xBF,
 uEchoProfileNodeProfileClassNumberOfSelfNodeInstances = 0xD3,
 uEchoProfileNodeProfileClassNumberOfSelfNodeClasses   = 0xD4,
 uEchoProfileNodeProfileClassInstanceListNotification  = 0xD5,
 uEchoProfileNodeProfileClassSelfNodeInstanceListS     = 0xD6,
 uEchoProfileNodeProfileClassSelfNodeClassListS        = 0xD7,
*/

uEchoObject *uecho_object_nodeprofileclass_new(void)
{
  uEchoObject *obj;
  byte propData[32];
  
  obj = uecho_object_new();
  if (!obj)
    return NULL;
  
  // Operation Status
  
  propData[0] = uEchoProfileNodeProfileClassBooting;
  uecho_object_addproperty(obj, uEchoProfileNodeProfileClassOperatingStatus, uEchoPropertyAttrRead, propData, uEchoProfileNodeProfileClassOperatingStatusLen);

  // Version Information
  
  propData[0] = uEchoMajorVersion;
  propData[1] = uEchoMinorVersion;
  propData[2] = uEchoSpecifiedMessageFormat;
  propData[3] = 0x00;
  uecho_object_addproperty(obj, uEchoProfileNodeProfileClassVersionInformation, uEchoPropertyAttrRead, propData, uEchoProfileNodeProfileClassVersionInformationLen);
  
  return obj;
}
