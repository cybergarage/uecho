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
 * uecho_profile_superclass_addmandatoryproperties
 ****************************************/

bool uecho_profile_superclass_addmandatoryproperties(uEchoObject *obj)
{
  byte manufactureCode[] = {0, 0, 0};
  
  if (!obj)
    return false;
  
  // Manufacture Code
  
  uecho_object_addproperty(obj, uEchoProfileSuperClassManufacturerCode, uEchoPropertyAttrRead,  manufactureCode, sizeof(manufactureCode));
  
  // Property map properties
  
  uecho_object_superclass_addmandatoryproperties(obj);
  
  return true;
}

/****************************************
 * uecho_profile_superclass_setmanufacturercode
 ****************************************/

bool uecho_profile_superclass_setmanufacturercode(uEchoObject *obj, byte *codes)
{
  return uecho_object_updatepropertydata(obj, uEchoProfileSuperClassManufacturerCode, codes, uEchoProfileSuperClassManufacturerCodeLen);
}
