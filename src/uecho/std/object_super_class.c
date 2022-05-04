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
 * uecho_object_addmandatoryproperties
 ****************************************/

bool uecho_object_addmandatoryproperties(uEchoObject* obj)
{
  byte zero_prop_map[] = { 0 };

  if (!obj)
    return false;

  // Manufacture Code

  uecho_object_setproperty(obj, uEchoObjectManufacturerCode, uEchoPropertyAttrRead);
  uecho_object_setmanufacturercode(obj, uEchoManufactureCodeDefault);

  // Property map properties

  uecho_object_setproperty(obj, uEchoObjectGetPropertyMap, uEchoPropertyAttrRead);
  uecho_object_setpropertydata(obj, uEchoObjectGetPropertyMap, zero_prop_map, sizeof(zero_prop_map));

  uecho_object_setproperty(obj, uEchoObjectSetPropertyMap, uEchoPropertyAttrRead);
  uecho_object_setpropertydata(obj, uEchoObjectSetPropertyMap, zero_prop_map, sizeof(zero_prop_map));

  uecho_object_setproperty(obj, uEchoObjectAnnoPropertyMap, uEchoPropertyAttrRead);
  uecho_object_setpropertydata(obj, uEchoObjectAnnoPropertyMap, zero_prop_map, sizeof(zero_prop_map));

  return true;
}

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
  if (!uecho_object_getpropertyintegerdata(obj, uEchoObjectManufacturerCode, uEchoObjectManufacturerCodeLen, &code))
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
      obj->get_prop_map_size++;
      obj->get_prop_map_bytes = realloc(obj->get_prop_map_bytes, obj->get_prop_map_size);
      if (obj->get_prop_map_bytes) {
        obj->get_prop_map_bytes[obj->get_prop_map_size - 1] = uecho_property_getcode(prop);
      }
    }

    // Set property map
    if (uecho_property_iswritable(prop)) {
      obj->set_prop_map_size++;
      obj->set_prop_map_bytes = realloc(obj->set_prop_map_bytes, obj->set_prop_map_size);
      if (obj->set_prop_map_bytes) {
        obj->set_prop_map_bytes[obj->set_prop_map_size - 1] = uecho_property_getcode(prop);
      }
    }

    // Announcement status changes property map
    if (uecho_property_isannounceable(prop)) {
      obj->anno_prop_map_size++;
      obj->anno_prop_map_bytes = realloc(obj->anno_prop_map_bytes, obj->anno_prop_map_size);
      if (obj->anno_prop_map_bytes) {
        obj->anno_prop_map_bytes[obj->anno_prop_map_size - 1] = uecho_property_getcode(prop);
      }
    }
  }

  // Update property map properties

  uecho_object_setpropertymap(obj, uEchoProfileGetPropertyMap, obj->get_prop_map_bytes, obj->get_prop_map_size);
  uecho_object_setpropertymap(obj, uEchoProfileSetPropertyMap, obj->set_prop_map_bytes, obj->set_prop_map_size);
  uecho_object_setpropertymap(obj, uEchoProfileAnnoPropertyMap, obj->anno_prop_map_bytes, obj->anno_prop_map_size);

  return true;
}

/****************************************
 * uecho_object_clearpropertymapcaches
 ****************************************/

void uecho_object_clearpropertymapcaches(uEchoObject* obj)
{
  if (!obj)
    return;

  if (obj->anno_prop_map_bytes) {
    free(obj->anno_prop_map_bytes);
    obj->anno_prop_map_bytes = NULL;
  }
  obj->anno_prop_map_size = 0;

  if (obj->set_prop_map_bytes) {
    free(obj->set_prop_map_bytes);
    obj->set_prop_map_bytes = NULL;
  }
  obj->set_prop_map_size = 0;

  if (obj->get_prop_map_bytes) {
    free(obj->get_prop_map_bytes);
    obj->get_prop_map_bytes = NULL;
  }
  obj->get_prop_map_bytes = 0;
}
