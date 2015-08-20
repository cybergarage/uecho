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
 * uecho_profile_addmandatoryproperties
 ****************************************/

bool uecho_profile_addmandatoryproperties(uEchoObject *obj)
{
  if (!obj)
    return false;

  uecho_object_addmandatoryproperties(obj);
  
  return true;
}

/****************************************
 * uecho_profile_setmanufacturercode
 ****************************************/

bool uecho_profile_setmanufacturercode(uEchoObject *obj, byte *codes)
{
  return uecho_object_setmanufacturercode(obj, codes);
}
