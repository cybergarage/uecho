/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
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

bool uecho_propertylist_set(uEchoPropertyList *props, uEchoPropertyCode code, uEchoPropertyAttr attr, byte *data, size_t dataLen)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_getbycode(props, code);
  if (!prop)
    return false;
  
  uecho_property_setdata(prop, data, dataLen);
  uecho_property_setattribute(prop, attr);

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
 * uecho_propertylist_setattribute
 ****************************************/

bool uecho_propertylist_setattribute(uEchoPropertyList *props, uEchoPropertyCode code, uEchoPropertyAttr attr)
{
  uEchoProperty *prop;
  
  prop = uecho_propertylist_getbycode(props, code);
  if (!prop)
    return false;
  
  uecho_property_setattribute(prop, attr);
  
  return true;
}
