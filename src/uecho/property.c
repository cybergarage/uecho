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

#include <uecho/property_internal.h>
#include <uecho/misc.h>

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
  
  prop->data = NULL;
  prop->dataSize = 0;
  
  uecho_property_setparentobject(prop, NULL);
  uecho_property_setattribute(prop, uEchoPropertyAttrReadWrite);
  
  return prop;
}

/****************************************
* uecho_property_delete
****************************************/

bool uecho_property_delete(uEchoProperty *prop)
{
  if (!prop)
    return false;
  
  uecho_property_cleardata(prop);
  uecho_property_remove(prop);

  free(prop);
  
  return true;
}

/****************************************
 * uecho_property_setparentobject
 ****************************************/

void uecho_property_setparentobject(uEchoProperty *prop, uEchoObject *obj)
{
  if (!prop)
    return;
  
  prop->parentObj = obj;
}

/****************************************
 * uecho_property_getparentobject
 ****************************************/

uEchoObject *uecho_property_getparentobject(uEchoProperty *prop)
{
  if (!prop)
    return NULL;
  
  return prop->parentObj;
}

/****************************************
 * uecho_property_setcount
 ****************************************/

bool uecho_property_setcount(uEchoProperty *prop, size_t count)
{
  uecho_property_cleardata(prop);
  
  if (!prop)
    return false;
  
  if (count == 0)
    return true;
  
  prop->data = (byte *)calloc(1, count);
  if (!prop->data)
    return false;
  
  prop->dataSize = count;
  
  return true;
}

/****************************************
 * uecho_property_setdata
 ****************************************/

bool uecho_property_setdata(uEchoProperty *prop, const byte *data, size_t count)
{
  if (!prop)
    return false;
  
  if (!uecho_property_setcount(prop, count))
    return false;
  
  if (count == 0)
    return true;
  
  memcpy(prop->data, data, count);

  return true;
}

/****************************************
 * uecho_property_setintegerdata
 ****************************************/

bool uecho_property_setintegerdata(uEchoProperty *prop, int data, size_t dataSize)
{
  bool isSuccess;
  byte *intByte;
  
  if (!prop)
    return false;

  intByte = (byte *)malloc(dataSize);
  if (!intByte)
    return true;
  
  uecho_integer2byte(data, intByte, dataSize);
  
  isSuccess = uecho_property_setdata(prop, intByte, dataSize);
  
  free(intByte);
  
  return isSuccess;
}

/****************************************
 * uecho_property_getintegerdata
 ****************************************/

bool uecho_property_getintegerdata(uEchoProperty *prop, size_t dataSize, int *data)
{
  if (!prop)
    return false;
  
  if (prop->dataSize != dataSize)
    return false;
  
  *data = uecho_byte2integer(prop->data, dataSize);
  
  return true;
}

/****************************************
 * uecho_property_setbytedata
 ****************************************/

bool uecho_property_setbytedata(uEchoProperty *prop, byte data)
{
  if (!prop)
    return false;

  return uecho_property_setdata(prop, &data, 1);
}

/****************************************
 * uecho_property_getbytedata
 ****************************************/

bool uecho_property_getbytedata(uEchoProperty *prop, byte *data)
{
  if (!prop)
    return false;
  
  if (prop->dataSize != 1)
    return false;
  
  *data = prop->data[0];
  
  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

bool uecho_property_cleardata(uEchoProperty *prop)
{
  if (!prop)
    return false;

  prop->dataSize= 0;
  
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
 * uecho_property_getdata
 ****************************************/

byte *uecho_property_getdata(uEchoProperty *prop)
{
  return prop->data;
}

/****************************************
 * uecho_property_getdatasize
 ****************************************/

byte uecho_property_getdatasize(uEchoProperty *prop)
{
  return prop->dataSize;
}

/****************************************
 * uecho_property_setattribute
 ****************************************/

void uecho_property_setattribute(uEchoProperty *prop, uEchoPropertyAttr val)
{
  prop->attr = val;
}

/****************************************
 * uecho_property_getattribute
 ****************************************/

uEchoPropertyAttr uecho_property_getattribute(uEchoProperty *prop)
{
  return prop->attr;
}

/****************************************
 * uecho_property_isreadable
 ****************************************/

bool uecho_property_isreadable(uEchoProperty *prop)
{
  return (prop->attr & uEchoPropertyAttrRead) ? true : false;
}

/****************************************
 * uecho_property_iswritable
 ****************************************/

bool uecho_property_iswritable(uEchoProperty *prop)
{
  return (prop->attr & uEchoPropertyAttrWrite) ? true : false;
}

/****************************************
 * uecho_property_isreadonly
 ****************************************/

bool uecho_property_isreadonly(uEchoProperty *prop)
{
  if (!prop)
    return false;

  if (!(prop->attr & uEchoPropertyAttrRead))
    return false;
  if ((prop->attr & uEchoPropertyAttrWrite))
    return false;
  
  return true;
}

/****************************************
 * uecho_property_iswriteonly
 ****************************************/

bool uecho_property_iswriteonly(uEchoProperty *prop)
{
  if (!prop)
    return false;
  
  if (!(prop->attr & uEchoPropertyAttrWrite))
    return false;
  if ((prop->attr & uEchoPropertyAttrRead))
    return false;

  return true;
}

/****************************************
 * uecho_property_isannouncement
 ****************************************/

bool uecho_property_isannouncement(uEchoProperty *prop)
{
  if (!prop)
    return false;
  
  if ((prop->attr & uEchoPropertyAttrAnno))
    return true;
  
  return false;
}

/****************************************
 * uecho_property_copy
 ****************************************/

uEchoProperty *uecho_property_copy(uEchoProperty *srcProp)
{
  uEchoProperty *newProp;
  
  if (!srcProp)
    return NULL;
  
  newProp = uecho_property_new();
  
  if (!newProp)
    return NULL;
  
  uecho_property_setcode(newProp, uecho_property_getcode(srcProp));
  uecho_property_setattribute(newProp, uecho_property_getattribute(srcProp));
  uecho_property_setdata(newProp, uecho_property_getdata(srcProp), uecho_property_getdatasize(srcProp));
  
  return newProp;
}

/****************************************
 * uecho_property_equals
 ****************************************/

bool uecho_property_equals(uEchoProperty *prop1, uEchoProperty *prop2)
{
  if (!prop1 || !prop2)
    return false;
  
  if (uecho_property_getcode(prop1) != uecho_property_getcode(prop2))
    return false;

  if (uecho_property_getattribute(prop1) != uecho_property_getattribute(prop2))
    return false;

  if (uecho_property_getdatasize(prop1) != uecho_property_getdatasize(prop2))
    return false;

  if (memcmp(uecho_property_getdata(prop1), uecho_property_getdata(prop2), uecho_property_getdatasize(prop1)) != 0)
    return false;

  return true;
}


