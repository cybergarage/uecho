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
 * uecho_object_addmandatoryproperties
 ****************************************/

bool uecho_object_addmandatoryproperties(uEchoObject *obj)
{
  byte manufactureCode[] = {0, 0, 0};
  byte zeroPropMap[] = {0};
  
  if (!obj)
    return false;
  
  // Manufacture Code
  
  uecho_object_addproperty(obj, uEchoObjectManufacturerCode, uEchoPropertyAttrRead,  manufactureCode, sizeof(manufactureCode));
  
  // Property map properties
  
  if (!uecho_object_addproperty(obj, uEchoObjectGetPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  if (!uecho_object_addproperty(obj, uEchoObjectSetPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  if (!uecho_object_addproperty(obj, uEchoObjectAnnoPropertyMap, uEchoPropertyAttrRead, zeroPropMap, sizeof(zeroPropMap)))
    return false;
  
  return true;
}

/****************************************
 * uecho_object_setmanufacturercode
 ****************************************/

bool uecho_object_setmanufacturercode(uEchoObject *obj, byte *codes)
{
  return uecho_object_updatepropertydata(obj, uEchoObjectManufacturerCode, codes, uEchoObjectManufacturerCodeLen);
}

/****************************************
 * uecho_object_updatepropertymaps
 ****************************************/

bool uecho_object_updatepropertymaps(uEchoObject *obj)
{
  uEchoProperty *prop;
  
  uecho_object_clearpropertymapcaches(obj);
  
  // Update property map caches
  
  for (prop = uecho_object_getproperties(obj); prop; prop = uecho_property_next(prop)) {
    // Get property map
    if (uecho_property_isreadable(prop)) {
      obj->getPropMapSize++;
      obj->getPropMapBytes = realloc(obj->getPropMapBytes, obj->getPropMapSize);
      if (obj->getPropMapBytes) {
        obj->getPropMapBytes[obj->getPropMapSize-1] = uecho_property_getcode(prop);
      }
    }
    
    // Set property map
    if (uecho_property_iswritable(prop)) {
      obj->setPropMapSize++;
      obj->setPropMapBytes = realloc(obj->setPropMapBytes, obj->setPropMapSize);
      if (obj->setPropMapBytes) {
        obj->setPropMapBytes[obj->setPropMapSize-1] = uecho_property_getcode(prop);
      }
    }
    
    // Announcement status changes property map
    if (uecho_property_isannouncement(prop)) {
      obj->annoPropMapSize++;
      obj->annoPropMapBytes = realloc(obj->annoPropMapBytes, obj->annoPropMapSize);
      if (obj->annoPropMapBytes) {
        obj->annoPropMapBytes[obj->annoPropMapSize-1] = uecho_property_getcode(prop);
      }
    }
  }
  
  // Update property map properties
  
  if (!uecho_object_updateproperty(obj, uEchoProfileGetPropertyMap, uEchoPropertyAttrRead, obj->getPropMapBytes, obj->getPropMapSize))
    return false;
  if (!uecho_object_updateproperty(obj, uEchoProfileSetPropertyMap, uEchoPropertyAttrRead, obj->setPropMapBytes, obj->setPropMapSize))
    return false;
  if (!uecho_object_updateproperty(obj, uEchoProfileAnnoPropertyMap, uEchoPropertyAttrRead, obj->annoPropMapBytes, obj->annoPropMapSize))
    return false;
  
  return true;
}

/****************************************
 * uecho_object_clearpropertymapcaches
 ****************************************/

void uecho_object_clearpropertymapcaches(uEchoObject *obj)
{
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
  obj->getPropMapSize = 0;
}

