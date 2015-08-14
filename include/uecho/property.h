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
  uEchoPropertyCodeMin = 0,
  uEchoPropertyCodeMax = 255,
};
  
/****************************************
 * Data Type
 ****************************************/

typedef byte uEchoPropertyCode;

typedef enum {
  uEchoPropertyPermNone      = 0x00,
  uEchoPropertyPermRead      = 0x01,
  uEchoPropertyPermWrite     = 0x10,
  uEchoPropertyPermReadWrite = uEchoPropertyPermRead | uEchoPropertyPermWrite,
} uEchoPropertyPerm;

typedef struct _uEchoProperty
{
  bool headFlag;
  struct _uEchoProperty *prev;
  struct _uEchoProperty *next;
  
  uEchoPropertyCode code;
  byte *data;
  size_t count;
  
  uEchoPropertyPerm perm;
  bool anno;
} uEchoProperty, uEchoPropertyList;

/****************************************
 * Function
 ****************************************/

uEchoProperty *uecho_property_new(void);
void uecho_property_delete(uEchoProperty *prop);

bool uecho_property_setcount(uEchoProperty *prop, size_t count);
bool uecho_property_setdata(uEchoProperty *prop, const byte *data, size_t count);
bool uecho_property_cleardata(uEchoProperty *prop);
  
uEchoProperty *uecho_property_next(uEchoProperty *prop);
void uecho_property_remove(uEchoProperty *prop);

void uecho_property_setcode(uEchoProperty *prop, uEchoPropertyCode val);
uEchoPropertyCode uecho_property_getcode(uEchoProperty *prop);

byte uecho_property_getcount(uEchoProperty *prop);
byte *uecho_property_getdata(uEchoProperty *prop);

void uecho_property_setpermission(uEchoProperty *prop, uEchoPropertyPerm val);
uEchoPropertyPerm uecho_property_getpermission(uEchoProperty *prop);
bool uecho_property_isreadable(uEchoProperty *prop);
bool uecho_property_iswritable(uEchoProperty *prop);
bool uecho_property_isreadonly(uEchoProperty *prop);
bool uecho_property_iswriteonly(uEchoProperty *prop);
  
void uecho_property_setannouncement(uEchoProperty *prop, bool flag);
bool uecho_property_isannouncement(uEchoProperty *prop);
  
/****************************************
 * Function (Object List)
 ****************************************/
  
uEchoPropertyList *uecho_propertylist_new(void);
void uecho_propertylist_delete(uEchoPropertyList *props);

bool uecho_propertylist_set(uEchoPropertyList *props, uEchoPropertyCode code, byte *data, size_t dataLen, uEchoPropertyPerm perm, bool annoFlag);
bool uecho_propertylist_setdata(uEchoPropertyList *props, uEchoPropertyCode code, byte *data, size_t dataLen);
bool uecho_propertylist_setpermission(uEchoPropertyList *props, uEchoPropertyCode code, uEchoPropertyPerm perm);
bool uecho_propertylist_setannouncement(uEchoPropertyList *props, uEchoPropertyCode code, bool annoFlag);
uEchoProperty *uecho_propertylist_findbycode(uEchoPropertyList *props, uEchoPropertyCode code);
  
#define uecho_propertylist_clear(props) uecho_list_clear((uEchoList *)props, (UECHO_LIST_DESTRUCTORFUNC)uecho_property_delete)
#define uecho_propertylist_size(props) uecho_list_size((uEchoList *)props)
#define uecho_propertylist_gets(props) (uEchoProperty *)uecho_list_next((uEchoList *)props)
#define uecho_propertylist_add(props,prop) uecho_list_add((uEchoList *)props, (uEchoList *)prop)
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
