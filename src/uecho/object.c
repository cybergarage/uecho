/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/object.h>
#include <uecho/profile.h>

/****************************************
* uecho_object_new
****************************************/

uEchoObject *uecho_object_new(void)
{
	uEchoObject *obj;

	obj = (uEchoObject *)malloc(sizeof(uEchoObject));
    
  if (!obj)
    return NULL;

  uecho_list_node_init((uEchoList *)obj);

  uecho_object_setclassgroupcode(obj, 0);
  uecho_object_setclasscode(obj, 0);
  uecho_object_setinstancecode(obj, 0);

  obj->properties = uecho_propertylist_new();
  uecho_object_addsuperclassproperties(obj);
  
	return obj;
}

/****************************************
* uecho_object_delete
****************************************/

void uecho_object_delete(uEchoObject *obj)
{
	uecho_list_remove((uEchoList *)obj);
    
  uecho_propertylist_delete(obj->properties);
	
  free(obj);
}

/****************************************
 * uecho_object_setcode
 ****************************************/

void uecho_object_setcode(uEchoObject *obj, uEchoObjectCode val)
{
  obj->code[0] = (val & 0xFF0000) >> 16;
  obj->code[1] = (val & 0x00FF00) >>  8;
  obj->code[2] = (val & 0x0000FF);
}

/****************************************
 * uecho_object_getcode
 ****************************************/

uEchoObjectCode uecho_object_getcode(uEchoObject *obj)
{
  int code = 0;
  code |= (obj->code[0] << 16) & 0xFF0000;
  code |= (obj->code[1] <<  8) & 0x00FF00;
  code |= (obj->code[2]      ) & 0x0000FF;
  return code;
}

/****************************************
 * uecho_object_setclassgroupcode
 ****************************************/

void uecho_object_setclassgroupcode(uEchoObject *obj, byte val)
{
  obj->code[0] = val;
}
  
/****************************************
 * uecho_object_getclassgroupcode
 ****************************************/

byte uecho_object_getclassgroupcode(uEchoObject *obj)
{
  return obj->code[0];
}

/****************************************
 * uecho_object_setclasscode
 ****************************************/

void uecho_object_setclasscode(uEchoObject *obj, byte val)
{
  obj->code[1] = val;
}
      
/****************************************
 * uecho_object_getclasscode
 ****************************************/

byte uecho_object_getclasscode(uEchoObject *obj)
{
  return obj->code[1];
}

/****************************************
 * uecho_object_setinstancecode
 ****************************************/

void uecho_object_setinstancecode(uEchoObject *obj, byte val)
{
  obj->code[2] = val;
}
          
/****************************************
 * uecho_object_getinstancecode
 ****************************************/

byte uecho_object_getinstancecode(uEchoObject *obj)
{
  return obj->code[2];
}

/****************************************
 * uecho_object_setproperty
 ****************************************/

bool uecho_object_setproperty(uEchoObject *obj, uEchoPropertyCode code, byte *data, size_t dataLen)
{
  return uecho_propertylist_set(obj->properties, code, data, dataLen);
}

/****************************************
 * uecho_object_getproperties
 ****************************************/

uEchoProperty *uecho_object_getproperties(uEchoObject *obj)
{
  return uecho_propertylist_gets(obj->properties);
}

/****************************************
 * uecho_object_getpropertybycode
 ****************************************/

uEchoProperty *uecho_object_getpropertybycode(uEchoObject *obj, uEchoPropertyCode code)
{
  return uecho_propertylist_getbycode(obj->properties, code);
}

/****************************************
 * uecho_object_getpropertycount
 ****************************************/

size_t uecho_object_getpropertycount(uEchoObject *obj)
{
  return uecho_propertylist_size(obj->properties);
}

/****************************************
 * uecho_object_clearproperties
 ****************************************/

void uecho_object_clearproperties(uEchoObject *obj)
{
  uecho_propertylist_clear(obj->properties);
}
