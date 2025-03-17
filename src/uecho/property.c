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

#include <uecho/_node.h>
#include <uecho/_property.h>
#include <uecho/profile.h>

#include <uecho/misc.h>

/****************************************
 * uecho_property_new
 ****************************************/

uEchoProperty* uecho_property_new(void)
{
  uEchoProperty* prop;

  prop = (uEchoProperty*)malloc(sizeof(uEchoProperty));

  if (!prop)
    return NULL;

  uecho_list_node_init((uEchoList*)prop);

  prop->name = uecho_string_new();
  prop->code = 0x00;
  prop->data = NULL;
  prop->data_size = 0;

  uecho_property_setparentobject(prop, NULL);
  uecho_property_setattribute(prop, uEchoPropertyAttrReadWrite);
  uecho_property_settype(prop, uEchoPropertyTypeNone);

  if (!prop->name) {
    uecho_property_delete(prop);
    return NULL;
  }

  return prop;
}

/****************************************
 * uecho_property_delete
 ****************************************/

bool uecho_property_delete(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if (prop->name) {
    uecho_string_delete(prop->name);
  }

  uecho_property_cleardata(prop);
  uecho_property_remove(prop);

  free(prop);

  return true;
}

/****************************************
 * uecho_property_setparentobject
 ****************************************/

bool uecho_property_setparentobject(uEchoProperty* prop, uEchoObject* obj)
{
  if (!prop)
    return false;

  prop->parentObj = obj;

  return true;
}

/****************************************
 * uecho_property_getparentobject
 ****************************************/

uEchoObject* uecho_property_getparentobject(uEchoProperty* prop)
{
  if (!prop)
    return NULL;

  return prop->parentObj;
}

/****************************************
 * uecho_property_setcount
 ****************************************/

bool uecho_property_setcount(uEchoProperty* prop, size_t dataSize)
{
  uecho_property_cleardata(prop);

  if (!prop)
    return false;

  if (dataSize == 0)
    return true;

  prop->data = (byte*)calloc(1, dataSize);
  if (!prop->data)
    return false;

  prop->data_size = dataSize;

  return true;
}

/****************************************
 * uecho_property_addcount
 ****************************************/

bool uecho_property_addcount(uEchoProperty* prop, size_t dataSize)
{
  size_t newDataSize;

  if (!prop)
    return false;

  if (dataSize == 0)
    return true;

  newDataSize = prop->data_size + dataSize;
  prop->data = (byte*)realloc(prop->data, newDataSize);
  if (!prop->data)
    return false;

  prop->data_size = newDataSize;

  return true;
}

/****************************************
 * uecho_property_setdata
 ****************************************/

bool uecho_property_setdata(uEchoProperty* prop, const byte* data, size_t dataSize)
{
  if (!prop)
    return false;

  if (!uecho_property_setcount(prop, dataSize))
    return false;

  if (dataSize == 0)
    return true;

  memcpy(prop->data, data, dataSize);

  // (D) Basic sequence for autonomous notification

  if (uecho_property_isannounceable(prop)) {
    uecho_property_announce(prop);
  }

  return true;
}

/****************************************
 * uecho_property_adddata
 ****************************************/

bool uecho_property_adddata(uEchoProperty* prop, const byte* data, size_t dataSize)
{
  size_t currDataSize;

  if (!prop)
    return false;

  if (dataSize == 0)
    return true;

  currDataSize = uecho_property_getdatasize(prop);

  if (!uecho_property_addcount(prop, dataSize))
    return false;

  memcpy((prop->data + currDataSize), data, dataSize);

  // (D) Basic sequence for autonomous notification

  if (uecho_property_isannounceable(prop)) {
    uecho_property_announce(prop);
  }

  return true;
}

/****************************************
 * uecho_property_addbytedata
 ****************************************/

bool uecho_property_addbytedata(uEchoProperty* prop, const byte data)
{
  return uecho_property_adddata(prop, &data, 1);
}

/****************************************
 * uecho_property_setintegerdata
 ****************************************/

bool uecho_property_setintegerdata(uEchoProperty* prop, int data, size_t dataSize)
{
  bool isSuccess;
  byte* intByte;

  if (!prop)
    return false;

  intByte = (byte*)malloc(dataSize);
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

bool uecho_property_getintegerdata(uEchoProperty* prop, int* data)
{
  if (!prop)
    return false;

  *data = uecho_byte2integer(prop->data, prop->data_size);

  return true;
}

/****************************************
 * uecho_property_setbytedata
 ****************************************/

bool uecho_property_setbytedata(uEchoProperty* prop, byte data)
{
  if (!prop)
    return false;

  return uecho_property_setdata(prop, &data, 1);
}

/****************************************
 * uecho_property_getbytedata
 ****************************************/

bool uecho_property_getbytedata(uEchoProperty* prop, byte* data)
{
  if (!prop)
    return false;

  if (prop->data_size != 1)
    return false;

  *data = prop->data[0];

  return true;
}

/****************************************
 * uecho_property_getpropertymapcount
 ****************************************/

bool uecho_property_getpropertymapcount(uEchoProperty* prop, size_t* count)
{
  if (!prop)
    return false;

  switch (prop->code) {
  case uEchoObjectAnnoPropertyMap:
  case uEchoObjectSetPropertyMap:
  case uEchoObjectGetPropertyMap: {
    if (prop->data_size < 1)
      return false;
    *count = prop->data[0];
    return true;
  } break;
  }

  return false;
}

/****************************************
 * uecho_property_getpropertymapcodes
 ****************************************/

uEchoPropertyCode uecho_propertymap_format2bittocode(int row, int bit)
{
  uEchoPropertyCode code;
  // 0 <= bit <= 7
  code = (0x10 * bit) + uEchoPropertyCodeMin;
  // 0 <= row <= 15
  code += row;
  return code;
}

bool uecho_property_getpropertymapcodes(uEchoProperty* prop, uEchoPropertyCode* propCodes, size_t propCodesSize)
{
  size_t propCodeCount, propCodeIdx;
  byte propByteCode, propByteBit;

  if (!uecho_property_getpropertymapcount(prop, &propCodeCount)) {
    return false;
  }
  if (propCodeCount != propCodesSize) {
    return false;
  }

  // Description Format 1

  if (propCodesSize <= uEchoPropertyMapFormat1MaxSize) {
    if (prop->data_size < (propCodeCount + 1)) {
      return false;
    }
    for (int n = 0; n < propCodeCount; n++) {
      propCodes[n] = prop->data[n + 1];
    }
    return true;
  }

  // Description Format 2

  if (prop->data_size != uEchoPropertyMapFormat2Size) {
    return false;
  }

  propCodeIdx = 0;
  for (int i = 0; i < uEchoPropertyMapFormat2MapSize; i++) {
    propByteCode = prop->data[i + 1];
    for (int j = 0; j < 8; j++) {
      propByteBit = (0x01 << j) & 0x0F;
      if ((propByteCode & propByteBit) == 0) {
        continue;
      }
      if (propCodesSize <= propCodeIdx) {
        return false;
      }
      propCodes[propCodeIdx] = uecho_propertymap_format2bittocode(i, j);
      propCodeIdx++;
    }
  }

  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

bool uecho_property_cleardata(uEchoProperty* prop)
{
  if (!prop)
    return false;

  prop->data_size = 0;

  if (prop->data) {
    free(prop->data);
    prop->data = NULL;
  }

  return true;
}

/****************************************
 * uecho_property_next
 ****************************************/

uEchoProperty* uecho_property_next(uEchoProperty* prop)
{
  return (uEchoProperty*)uecho_list_next((uEchoList*)prop);
}

/****************************************
 * uecho_property_remove
 ****************************************/

void uecho_property_remove(uEchoProperty* prop)
{
  uecho_list_remove((uEchoList*)prop);
}

/****************************************
 * uecho_property_setname
 ****************************************/

void uecho_property_setname(uEchoProperty* prop, const char* name)
{
  if (!prop)
    return;
  uecho_string_setvalue(prop->name, name);
}

/****************************************
 * uecho_property_getname
 ****************************************/

const char* uecho_property_getname(uEchoProperty* prop)
{
  if (!prop)
    return NULL;
  return uecho_string_getvalue(prop->name);
}

/****************************************
 * uecho_property_setcode
 ****************************************/

void uecho_property_setcode(uEchoProperty* prop, uEchoPropertyCode val)
{
  if (!prop)
    return;
  prop->code = val;
}

/****************************************
 * uecho_property_getcode
 ****************************************/

uEchoPropertyCode uecho_property_getcode(uEchoProperty* prop)
{
  if (!prop)
    return 0;
  return prop->code;
}

/****************************************
 * uecho_property_getdata
 ****************************************/

byte* uecho_property_getdata(uEchoProperty* prop)
{
  if (!prop)
    return NULL;
  return prop->data;
}

/****************************************
 * uecho_property_getdatasize
 ****************************************/

size_t uecho_property_getdatasize(uEchoProperty* prop)
{
  if (!prop)
    return 0;
  return prop->data_size;
}

/****************************************
 * uecho_property_setattribute
 ****************************************/

void uecho_property_setattribute(uEchoProperty* prop, uEchoPropertyAttr val)
{
  if (!prop)
    return;
  prop->attr = val;
}

/****************************************
 * uecho_property_getattribute
 ****************************************/

uEchoPropertyAttr uecho_property_getattribute(uEchoProperty* prop)
{
  if (!prop)
    return uEchoPropertyAttrNone;
  return prop->attr;
}

/****************************************
 * uecho_property_isreadable
 ****************************************/

bool uecho_property_isreadable(uEchoProperty* prop)
{
  if (!prop)
    return false;
  return (prop->attr & (uEchoPropertyAttrRead | uEchoPropertyAttrReadRequired)) ? true : false;
}

/****************************************
 * uecho_property_isreadrequired
 ****************************************/

bool uecho_property_isreadrequired(uEchoProperty* prop)
{
  if (!prop)
    return false;
  return (prop->attr & uEchoPropertyAttrReadRequired) ? true : false;
}

/****************************************
 * uecho_property_iswritable
 ****************************************/

bool uecho_property_iswritable(uEchoProperty* prop)
{
  if (!prop)
    return false;
  return (prop->attr & (uEchoPropertyAttrWrite | uEchoPropertyAttrWriteRequired)) ? true : false;
}

/****************************************
 * uecho_property_iswriterequired
 ****************************************/

bool uecho_property_iswriterequired(uEchoProperty* prop)
{
  if (!prop)
    return false;
  return (prop->attr & uEchoPropertyAttrWriteRequired) ? true : false;
}

/****************************************
 * uecho_property_isreadonly
 ****************************************/

bool uecho_property_isreadonly(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if (!uecho_property_isreadable(prop))
    return false;
  if (uecho_property_iswritable(prop))
    return false;

  return true;
}

/****************************************
 * uecho_property_iswriteonly
 ****************************************/

bool uecho_property_iswriteonly(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if (!uecho_property_iswritable(prop))
    return false;
  if (uecho_property_isreadable(prop))
    return false;

  return true;
}

/****************************************
 * uecho_property_isannounceable
 ****************************************/

bool uecho_property_isannounceable(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if ((prop->attr & (uEchoPropertyAttrAnno | uEchoPropertyAttrAnnoRequired)))
    return true;

  return false;
}

/****************************************
 * uecho_property_isannouncerequired
 ****************************************/

bool uecho_property_isannouncerequired(uEchoProperty* prop)
{
  if (!prop)
    return false;

  if ((prop->attr & uEchoPropertyAttrAnnoRequired))
    return true;

  return false;
}

/****************************************
 * uecho_property_settype
 ****************************************/

void uecho_property_settype(uEchoProperty* prop, uEchoPropertyType val)
{
  if (!prop)
    return;
  prop->type = val;
}

/****************************************
 * uecho_property_gettype
 ****************************************/

uEchoPropertyType uecho_property_gettype(uEchoProperty* prop)
{
  if (!prop)
    return uEchoPropertyTypeNone;
  return prop->type;
}

/****************************************
 * uecho_property_copy
 ****************************************/

uEchoProperty* uecho_property_copy(uEchoProperty* srcProp)
{
  uEchoProperty* newProp;

  if (!srcProp)
    return NULL;

  newProp = uecho_property_new();

  if (!newProp)
    return NULL;

  uecho_property_setcode(newProp, uecho_property_getcode(srcProp));
  uecho_property_setname(newProp, uecho_property_getname(srcProp));
  uecho_property_setattribute(newProp, uecho_property_getattribute(srcProp));
  uecho_property_setdata(newProp, uecho_property_getdata(srcProp), uecho_property_getdatasize(srcProp));

  return newProp;
}

/****************************************
 * uecho_property_equals
 ****************************************/

bool uecho_property_equals(uEchoProperty* prop1, uEchoProperty* prop2)
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

/****************************************
 * uecho_property_getnode
 ****************************************/

uEchoNode* uecho_property_getnode(uEchoProperty* prop)
{
  uEchoObject* parentObj;
  uEchoNode* parentNode;

  if (!prop)
    return NULL;

  parentObj = uecho_property_getparentobject(prop);
  if (!parentObj)
    return NULL;

  parentNode = uecho_object_getparentnode(parentObj);
  if (!parentNode)
    return NULL;

  return parentNode;
}

/****************************************
 * uecho_property_announce
 ****************************************/

bool uecho_property_announce(uEchoProperty* prop)
{
  uEchoNode* node;
  uEchoObject* obj;

  if (!prop)
    return false;

  node = uecho_property_getnode(prop);
  if (!node)
    return false;

  obj = uecho_property_getparentobject(prop);
  if (!obj)
    return false;

  return uecho_node_announceproperty(node, obj, prop);
}

/****************************************
 * uecho_property_isdataequal
 ****************************************/

bool uecho_property_isdataequal(uEchoProperty* prop, const byte* data, size_t dataSize)
{
  if (!prop)
    return false;

  if (uecho_property_getdatasize(prop) != dataSize)
    return false;

  return (memcmp(uecho_property_getdata(prop), data, dataSize) == 0) ? true : false;
}
