/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_OBJECT_H_
#define _UECHO_OBJECT_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/property.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoObjectCodeMin = 0x000000,
  uEchoObjectCodeMax = 0xFFFFFF,
};
  
enum {
  uEchoNodeProfileObject = 0x0EF001,
};
  
/****************************************
 * Data Type
 ****************************************/

typedef unsigned int uEchoObjectCode;
  
typedef struct _uEchoObject {
  bool headFlag;
  struct _uEchoObject *prev;
  struct _uEchoObject *next;

  byte code[3];
  uEchoPropertyList *properties;

  // Property map caches

  size_t annoPropMapSize;
  byte *annoPropMapBytes;
  
  size_t setPropMapSize;
  byte *setPropMapBytes;

  size_t getPropMapSize;
  byte *getPropMapBytes;
} uEchoObject, uEchoObjectList;

/****************************************
 * Function
 ****************************************/

uEchoObject *uecho_object_new(void);
void uecho_object_delete(uEchoObject *obj);
#define uecho_object_next(obj) (uEchoObject *)uecho_list_next((uEchoList *)obj)
#define uecho_object_remove(obj) uecho_list_remove((uEchoList *)obj)

void uecho_object_setcode(uEchoObject *obj, uEchoObjectCode val);
uEchoObjectCode uecho_object_getcode(uEchoObject *obj);
  
bool uecho_object_start(uEchoObject *obj);
bool uecho_object_stop(uEchoObject *obj);
bool uecho_object_isrunning(uEchoObject *obj);

void uecho_object_setclassgroupcode(uEchoObject *obj, byte val);
byte uecho_object_getclassgroupcode(uEchoObject *obj);

void uecho_object_setclasscode(uEchoObject *obj, byte val);
byte uecho_object_getclasscode(uEchoObject *obj);

void uecho_object_setinstancecode(uEchoObject *obj, byte val);
byte uecho_object_getinstancecode(uEchoObject *obj);

bool uecho_object_addproperty(uEchoObject *obj, uEchoPropertyCode code, uEchoPropertyAttr attr, byte *data, size_t dataLen);
bool uecho_object_updateproperty(uEchoObject *obj, uEchoPropertyCode code, uEchoPropertyAttr attr, byte *data, size_t dataLen);
bool uecho_object_updatepropertydata(uEchoObject *obj, uEchoPropertyCode code, byte *data, size_t dataLen);
bool uecho_object_updatepropertyattribute(uEchoObject *obj, uEchoPropertyCode code, uEchoPropertyAttr attr);

uEchoProperty *uecho_object_getproperties(uEchoObject *obj);
uEchoProperty *uecho_object_getproperty(uEchoObject *obj, uEchoPropertyCode code);
bool uecho_object_hasproperty(uEchoObject *obj, uEchoPropertyCode code);
size_t uecho_object_getpropertycount(uEchoObject *obj);
void uecho_object_clearproperties(uEchoObject *obj);

/****************************************
 * Function (Object List)
 ****************************************/
  
uEchoObjectList *uecho_objectlist_new(void);
void uecho_objectlist_delete(uEchoObjectList *objs);
  
bool uecho_objectlist_set(uEchoObjectList *props, uEchoObjectCode code);
uEchoObject *uecho_objectlist_getbycode(uEchoObjectList *props, uEchoObjectCode code);
  
#define uecho_objectlist_clear(objs) uecho_list_clear((uEchoList *)objs, (UECHO_LIST_DESTRUCTORFUNC)uecho_object_delete)
#define uecho_objectlist_size(objs) uecho_list_size((uEchoList *)objs)
#define uecho_objectlist_gets(objs) (uEchoObject *)uecho_list_next((uEchoList *)objs)
#define uecho_objectlist_add(objs,obj) uecho_list_add((uEchoList *)objs, (uEchoList *)obj)

/****************************************
 * Object Super Class
 ****************************************/
  
typedef enum {
  uEchoObjectManufacturerCode = 0x8A,
  uEchoObjectAnnoPropertyMap  = 0x9D,
  uEchoObjectSetPropertyMap   = 0x9E,
  uEchoObjectGetPropertyMap   = 0x9F,
} uEchoObjectEPC;
  
typedef enum {
  uEchoObjectManufacturerCodeLen   = 3,
  uEchoObjectPropertyMapMaxLen     = 16,
  uEchoObjectAnnoPropertyMapMaxLen =(uEchoObjectPropertyMapMaxLen + 1),
  uEchoObjectSetPropertyMapMaxLen  = (uEchoObjectPropertyMapMaxLen + 1),
  uEchoObjectGetPropertyMapMaxLen  = (uEchoObjectPropertyMapMaxLen + 1),
} uEchoObjectEPCSize;

bool uecho_object_addmandatoryproperties(uEchoObject *obj);
bool uecho_object_updatepropertymaps(uEchoObject *obj);
void uecho_object_clearpropertymapcaches(uEchoObject *obj);

bool uecho_object_setmanufacturercode(uEchoObject *obj, byte *codes);
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
