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

#include <uecho/profile.h>

/****************************************
 * uecho_object_addsuperclassproperties
 ****************************************/

bool uecho_object_addsuperclassproperties(uEchoObject *obj)
{
  byte manufactureCode[uEchoProfileObjectSuperClassManufacturerCodeLen] = {0, 0, 0};
  
  if (!obj)
    return false;
  
  uecho_object_setproperty(obj, uEchoProfileObjectSuperClassManufacturerCode, manufactureCode, sizeof(manufactureCode), uEchoPropertyPermRead, uEchoPropertyAnnouncementNone);
  
  return uecho_object_updatesuperclassproperties(obj);
}

/****************************************
 * uecho_object_addsuperclassproperties
 ****************************************/

bool uecho_object_updatesuperclassproperties(uEchoObject *obj)
{
  return true;
}
