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

#include <uecho/_object.h>
#include <uecho/profile.h>

/****************************************
 * uecho_object_setmanufacturercode
 ****************************************/

bool uecho_object_setmanufacturercode(uEchoObject* obj, uEchoManufactureCode code)
{
  return uecho_object_setpropertyintegerdata(obj, uEchoObjectManufacturerCode, code, uEchoObjectManufacturerCodeLen);
}

/****************************************
 * uecho_object_getmanufacturercode
 ****************************************/

uEchoManufactureCode uecho_object_getmanufacturercode(uEchoObject* obj)
{
  uEchoManufactureCode code;
  if (!uecho_object_getpropertyintegerdata(obj, uEchoObjectManufacturerCode, &code))
    return uEchoManufactureCodeDefault;
  return code;
}

/****************************************
 * uecho_object_updatepropertymaps
 ****************************************/

bool uecho_object_updatepropertymaps(uEchoObject* obj)
{
  uEchoProperty* prop;

  if (!obj)
    return false;

  uecho_object_clearpropertymapcaches(obj);

  // Update property map caches

  for (prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
    // Get property map
    if (uecho_property_isreadable(prop)) {
      obj->getPropMapSize++;
      obj->getPropMapBytes = realloc(obj->getPropMapBytes, obj->getPropMapSize);
      if (obj->getPropMapBytes) {
        obj->getPropMapBytes[obj->getPropMapSize - 1] = uecho_property_getcode(prop);
      }
    }

    // Set property map
    if (uecho_property_iswritable(prop)) {
      obj->setPropMapSize++;
      obj->setPropMapBytes = realloc(obj->setPropMapBytes, obj->setPropMapSize);
      if (obj->setPropMapBytes) {
        obj->setPropMapBytes[obj->setPropMapSize - 1] = uecho_property_getcode(prop);
      }
    }

    // Announcement status changes property map
    if (uecho_property_isannounceable(prop)) {
      obj->annoPropMapSize++;
      obj->annoPropMapBytes = realloc(obj->annoPropMapBytes, obj->annoPropMapSize);
      if (obj->annoPropMapBytes) {
        obj->annoPropMapBytes[obj->annoPropMapSize - 1] = uecho_property_getcode(prop);
      }
    }
  }

  // Update property map properties

  uecho_object_setpropertymap(obj, uEchoProfileGetPropertyMap, obj->getPropMapBytes, obj->getPropMapSize);
  uecho_object_setpropertymap(obj, uEchoProfileSetPropertyMap, obj->setPropMapBytes, obj->setPropMapSize);
  uecho_object_setpropertymap(obj, uEchoProfileAnnoPropertyMap, obj->annoPropMapBytes, obj->annoPropMapSize);

  return true;
}

/****************************************
 * uecho_object_clearpropertymapcaches
 ****************************************/

void uecho_object_clearpropertymapcaches(uEchoObject* obj)
{
  if (!obj)
    return;

  if (obj->annoPropMapBytes) {
    free(obj->annoPropMapBytes);
    obj->annoPropMapBytes = NULL;
  }
  obj->annoPropMapSize = 0;

  if (obj->setPropMapBytes) {
    free(obj->setPropMapBytes);
    obj->setPropMapBytes = NULL;
  }
  obj->setPropMapSize = 0;

  if (obj->getPropMapBytes) {
    free(obj->getPropMapBytes);
    obj->getPropMapBytes = NULL;
  }
  obj->getPropMapBytes = 0;
}
