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

#include <uecho/property.h>

/****************************************
* uecho_property_new
****************************************/

uEchoProperty *uecho_property_new(void)
{
	uEchoProperty *prop;

	prop = (uEchoProperty *)malloc(sizeof(uEchoProperty));
    
  if (!prop)
    return NULL;

  uecho_list_node_init((uEchoList *)prop);
  
  prop->count = 0;
  prop->data = NULL;
  
	return prop;
}

/****************************************
* uecho_property_delete
****************************************/

void uecho_property_delete(uEchoProperty *prop)
{
  uecho_property_cleardata(prop);
  uecho_property_remove(prop);

	free(prop);
}

/****************************************
 * uecho_property_setcount
 ****************************************/

bool uecho_property_setcount(uEchoProperty *prop, size_t count)
{
  uecho_property_cleardata(prop);
  
  if (count == 0)
    return true;
  
  prop->data = (byte *)calloc(1, count);
  if (!prop->data)
    return false;
  
  prop->count = count;
  
  return true;
}

/****************************************
 * uecho_property_setdata
 ****************************************/

bool uecho_property_setdata(uEchoProperty *prop, const byte *data, size_t count)
{
  if (!uecho_property_setcount(prop, count))
    return false;
  
  if (count == 0)
    return true;
  
  memcpy(prop->data, data, count);

  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

bool uecho_property_cleardata(uEchoProperty *prop)
{
  prop->count = 0;
  
  if (prop->data) {
    free(prop->data);
    prop->data = NULL;
  }
  
  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

uEchoProperty *uecho_property_next(uEchoProperty *prop)
{
  return (uEchoProperty *)uecho_list_next((uEchoList *)prop);
}
  
/****************************************
 * uecho_property_remove
 ****************************************/

void uecho_property_remove(uEchoProperty *prop)
{
  uecho_list_remove((uEchoList *)prop);
}

/****************************************
 * uecho_property_setcode
 ****************************************/

void uecho_property_setcode(uEchoProperty *prop, uEchoPropertyCode val)
{
  prop->code = val;
}
      
/****************************************
 * uecho_property_getcode
 ****************************************/

uEchoPropertyCode uecho_property_getcode(uEchoProperty *prop)
{
  return prop->code;
}

/****************************************
 * uecho_property_getcount
 ****************************************/

byte uecho_property_getcount(uEchoProperty *prop)
{
  return prop->count;
}

/****************************************
 * uecho_property_getdata
 ****************************************/

byte *uecho_property_getdata(uEchoProperty *prop)
{
  return prop->data;
}
