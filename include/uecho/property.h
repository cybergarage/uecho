/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
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

typedef struct _uEchoProperty
{
  bool headFlag;
  struct _uEchoProperty *prev;
  struct _uEchoProperty *next;
  
  byte code;
  size_t count;
  byte *data;
} uEchoProperty, uEchoPropertyList;

/****************************************
 * Function
 ****************************************/

uEchoProperty *uecho_property_new(void);
void uecho_property_delete(uEchoProperty *prop);

bool uecho_property_setcount(uEchoProperty *prop, size_t count);
bool uecho_property_setdata(uEchoProperty *prop, const byte *data, size_t count);
bool uecho_property_cleardata(uEchoProperty *prop);
  
/****************************************
 * Macro
 ****************************************/
  
#if defined(C99)

inline void uecho_property_setcode(uEchoProperty *prop, byte val) {prop->code = val;}
inline byte uecho_property_getcode(uEchoProperty *prop) {return prop->code;}

inline byte uecho_property_getcount(uEchoProperty *prop) {return prop->count;}

inline byte *uecho_property_getdata(uEchoProperty *prop) {return prop->data;}
  
#else
  
#define uecho_property_setcode(prop, val) (prop->code = val)
#define uecho_property_getcode(prop) (prop->code)

#define uecho_property_getcount(prop) (prop->count)

#define uecho_property_getdata(prop) (prop->data)

#endif
  
/****************************************
 * Function (Object List)
 ****************************************/
  
uEchoPropertyList *uecho_propertylist_new(void);
void uecho_propertylist_delete(uEchoPropertyList *props);

uEchoProperty *uecho_propertylist_find(uEchoPropertyList *props, uEchoProperty *prop);
bool uecho_propertylist_set(uEchoPropertyList *props, uEchoProperty *prop);
  
#define uecho_propertylist_clear(props) uecho_list_clear((uEchoList *)props, (UECHO_LIST_DESTRUCTORFUNC)uecho_property_delete)
#define uecho_propertylist_size(props) uecho_list_size((uEchoList *)props)
#define uecho_propertylist_gets(props) (uEchoProperty *)uecho_list_next((uEchoList *)props)
#define uecho_propertylist_add(props,prop) uecho_list_add((uEchoList *)props, (uEchoList *)prop)
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
