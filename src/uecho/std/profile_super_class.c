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

#include <uecho/object_internal.h>
#include <uecho/profile.h>

/****************************************
 * uecho_profile_addmandatoryproperties
 ****************************************/

bool uecho_profile_addmandatoryproperties(uEchoObject* obj)
{
  if (!obj)
    return false;

  uecho_object_addmandatoryproperties(obj);

  return true;
}

/****************************************
 * uecho_profile_setmanufacturercode
 ****************************************/

bool uecho_profile_setmanufacturercode(uEchoObject* obj, uEchoObjectCode code)
{
  return uecho_object_setmanufacturercode(obj, code);
}

/****************************************
 * uecho_profile_getmanufacturercode
 ****************************************/

bool uecho_profile_getmanufacturercode(uEchoObject* obj)
{
  return uecho_object_getmanufacturercode(obj);
}
