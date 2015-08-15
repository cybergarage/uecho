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
  
  obj = uecho_object_new();
  if (!obj)
    return NULL;
  
  /*
  uEchoPropertyData props[] = {
    {uEchoProfileNodeProfileClassOperatingStatus,uEchoPropertyAttrRead, "1", uEchoProfileNodeProfileClassOperatingStatusLen},
  };
   */
  
  //uecho_object_addproperty(obj, <#uEchoPropertyCode code#>, <#byte *data#>, <#size_t dataLen#>, <#uEchoPropertyAttr attr#>, <#bool annoFlag#>)

  return obj;
}