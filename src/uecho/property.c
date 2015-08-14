/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
  
  uecho_property_setpermission(prop, uEchoPropertyPermReadWrite);
  uecho_property_setannouncement(prop, false);
  
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

/****************************************
 * uecho_property_setpermission
 ****************************************/

void uecho_property_setpermission(uEchoProperty *prop, uEchoPropertyPerm val)
{
  prop->perm = val;
}

/****************************************
 * uecho_property_getpermission
 ****************************************/

uEchoPropertyPerm uecho_property_getpermission(uEchoProperty *prop)
{
  return prop->perm;
}

/****************************************
 * uecho_property_isreadable
 ****************************************/

bool uecho_property_isreadable(uEchoProperty *prop)
{
  return (prop->perm & uEchoPropertyPermRead) ? true : false;
}

/****************************************
 * uecho_property_iswritable
 ****************************************/

bool uecho_property_iswritable(uEchoProperty *prop)
{
  return (prop->perm & uEchoPropertyPermWrite) ? true : false;
}

/****************************************
 * uecho_property_isreadonly
 ****************************************/

bool uecho_property_isreadonly(uEchoProperty *prop)
{
  if (!(prop->perm & uEchoPropertyPermRead))
    return false;
  if ((prop->perm & uEchoPropertyPermWrite))
    return false;
  return true;
}

/****************************************
 * uecho_property_iswriteonly
 ****************************************/

bool uecho_property_iswriteonly(uEchoProperty *prop)
{
  if (!(prop->perm & uEchoPropertyPermWrite))
    return false;
  if ((prop->perm & uEchoPropertyPermRead))
    return false;
  return true;
}

/****************************************
 * uecho_property_setannouncement
 ****************************************/

void uecho_property_setannouncement(uEchoProperty *prop, bool flag)
{
  prop->anno = flag;
}

/****************************************
 * uecho_property_isannouncement
 ****************************************/

bool uecho_property_isannouncement(uEchoProperty *prop)
{
  return prop->anno;
}
