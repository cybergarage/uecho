/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/property.h>

/****************************************
* uecho_propertylist_new
****************************************/

uEchoPropertyList *uecho_propertylist_new(void)
{
	uEchoPropertyList *props;

	props = (uEchoPropertyList *)malloc(sizeof(uEchoPropertyList));
  if (!props)
    return NULL;

  uecho_list_header_init((uEchoList *)props);

	return props;
}

/****************************************
* uecho_propertylist_delete
****************************************/

void uecho_propertylist_delete(uEchoPropertyList *props)
{
	uecho_propertylist_clear(props);

	free(props);
}

/****************************************
 * uecho_propertylist_findbycode
 ****************************************/

uEchoProperty *uecho_propertylist_findbycode(uEchoPropertyList *props, uEchoPropertyCode code)
{
  uEchoProperty *prop;
  
  for (prop = uecho_propertylist_gets(props); prop; prop = uecho_property_next(prop)) {
    if (uecho_property_getcode(prop) == code)
      return prop;
  }
  
  return NULL;
}

/****************************************
 * uecho_propertylist_getbycode
 ****************************************/

uEchoProperty *uecho_propertylist_getbycode(uEchoPropertyList *props, uEchoPropertyCode code)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_findbycode(props, code);
  if (prop)
    return prop;
  
  prop = uecho_property_new();
  if (!prop)
    return NULL;
  
  uecho_property_setcode(prop, code);
  uecho_propertylist_add(props, prop);
  
  return prop;
}

/****************************************
 * uecho_propertylist_set
 ****************************************/

bool uecho_propertylist_set(uEchoPropertyList *props, uEchoPropertyCode code, byte *data, size_t dataLen, uEchoPropertyPerm perm, bool annoFlag)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_getbycode(props, code);
  if (!prop)
    return false;
  
  uecho_property_setdata(prop, data, dataLen);
  uecho_property_setpermission(prop, perm);
  uecho_property_setannouncement(prop, annoFlag);

  return true;
}

/****************************************
 * uecho_propertylist_setdata
 ****************************************/

bool uecho_propertylist_setdata(uEchoPropertyList *props, uEchoPropertyCode code, byte *data, size_t dataLen)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_getbycode(props, code);
  if (!prop)
    return false;

  uecho_property_setdata(prop, data, dataLen);
  
  return true;
}

/****************************************
 * uecho_propertylist_setpermission
 ****************************************/

bool uecho_propertylist_setpermission(uEchoPropertyList *props, uEchoPropertyCode code, uEchoPropertyPerm perm)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_getbycode(props, code);
  if (!prop)
    return false;
  
  uecho_property_setpermission(prop, perm);
  
  return true;
}

/****************************************
 * uecho_propertylist_setannouncement
 ****************************************/

bool uecho_propertylist_setannouncement(uEchoPropertyList *props, uEchoPropertyCode code, bool annoFlag)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_getbycode(props, code);
  if (!prop)
    return false;
  
  uecho_property_setannouncement(prop, annoFlag);
  
  return true;
}
