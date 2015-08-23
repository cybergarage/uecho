/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROPERTY_H_
#define _UECHO_PROPERTY_H_

#include <stdbool.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoPropertyCodeMin = 0x80,
  uEchoPropertyCodeMax = 0xFF,
};
  
/****************************************
 * Data Type
 ****************************************/

typedef byte uEchoPropertyCode;

typedef enum {
  uEchoPropertyAttrNone      = 0x00,
  uEchoPropertyAttrRead      = 0x01,
  uEchoPropertyAttrWrite     = 0x02,
  uEchoPropertyAttrAnno      = 0x10,
  uEchoPropertyAttrReadWrite = uEchoPropertyAttrRead | uEchoPropertyAttrWrite,
  uEchoPropertyAttrReadAnno  = uEchoPropertyAttrRead | uEchoPropertyAttrAnno,
} uEchoPropertyAttr;

typedef struct _uEchoProperty
{
  bool headFlag;
  struct _uEchoProperty *prev;
  struct _uEchoProperty *next;
  
  uEchoPropertyCode code;
  uEchoPropertyAttr attr;
  byte *data;
  size_t dataSize;
} uEchoProperty, uEchoPropertyList;

typedef struct
{
  uEchoPropertyCode code;
  uEchoPropertyAttr attr;
  byte *data;
  size_t dataSize;
} uEchoPropertyData;
  
/****************************************
 * Function
 ****************************************/

uEchoProperty *uecho_property_new(void);
void uecho_property_delete(uEchoProperty *prop);

bool uecho_property_setcount(uEchoProperty *prop, size_t count);
bool uecho_property_setdata(uEchoProperty *prop, const byte *data, size_t count);
bool uecho_property_setintegerdata(uEchoProperty *prop, int data, size_t dataSize);
bool uecho_property_setbytedata(uEchoProperty *prop, byte data);
bool uecho_property_cleardata(uEchoProperty *prop);
  
uEchoProperty *uecho_property_next(uEchoProperty *prop);
void uecho_property_remove(uEchoProperty *prop);

void uecho_property_setcode(uEchoProperty *prop, uEchoPropertyCode val);
uEchoPropertyCode uecho_property_getcode(uEchoProperty *prop);

byte uecho_property_getdatasize(uEchoProperty *prop);
byte *uecho_property_getdata(uEchoProperty *prop);
bool uecho_property_getintegerdata(uEchoProperty *prop, size_t dataSize, int *data);
bool uecho_property_getbytedata(uEchoProperty *prop, byte *data);

void uecho_property_setattribute(uEchoProperty *prop, uEchoPropertyAttr val);
uEchoPropertyAttr uecho_property_getattribute(uEchoProperty *prop);
bool uecho_property_isreadable(uEchoProperty *prop);
bool uecho_property_iswritable(uEchoProperty *prop);
bool uecho_property_isreadonly(uEchoProperty *prop);
bool uecho_property_iswriteonly(uEchoProperty *prop);
bool uecho_property_isannouncement(uEchoProperty *prop);
  
/****************************************
 * Function (Object List)
 ****************************************/
  
uEchoPropertyList *uecho_propertylist_new(void);
void uecho_propertylist_delete(uEchoPropertyList *props);

bool uecho_propertylist_set(uEchoPropertyList *props, uEchoPropertyCode code, uEchoPropertyAttr attr, byte *data, size_t dataLen);
bool uecho_propertylist_setdata(uEchoPropertyList *props, uEchoPropertyCode code, byte *data, size_t dataLen);
bool uecho_propertylist_setintegerdata(uEchoPropertyList *props, uEchoPropertyCode code, int data, size_t dataLen);
bool uecho_propertylist_setbytedata(uEchoPropertyList *props, uEchoPropertyCode code, byte data);
bool uecho_propertylist_setattribute(uEchoPropertyList *props, uEchoPropertyCode code, uEchoPropertyAttr attr);
uEchoProperty *uecho_propertylist_findbycode(uEchoPropertyList *props, uEchoPropertyCode code);
  
bool uecho_propertylist_getintegerdata(uEchoPropertyList *props, uEchoPropertyCode code, size_t dataLen, int *data);
bool uecho_propertylist_getbytedata(uEchoPropertyList *props, uEchoPropertyCode code, byte *data);
  
#define uecho_propertylist_clear(props) uecho_list_clear((uEchoList *)props, (UECHO_LIST_DESTRUCTORFUNC)uecho_property_delete)
#define uecho_propertylist_size(props) uecho_list_size((uEchoList *)props)
#define uecho_propertylist_gets(props) (uEchoProperty *)uecho_list_next((uEchoList *)props)
#define uecho_propertylist_add(props,prop) uecho_list_add((uEchoList *)props, (uEchoList *)prop)
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
