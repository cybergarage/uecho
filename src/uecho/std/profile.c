/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/profile.h>

/****************************************
* uecho_profile_new
****************************************/

uEchoObject *uecho_profile_new(void)
{
  uEchoObject *obj;

  obj = uecho_object_new();
    
  if (!obj)
    return NULL;
  
  uecho_profile_addmandatoryproperties(obj);

  return obj;
}
