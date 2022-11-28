/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROPERTY_INTERNAL_H_
#define _UECHO_PROPERTY_INTERNAL_H_

#include <stdbool.h>

#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/util/strings.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct {
  UECHO_LIST_STRUCT_MEMBERS
  uEchoString* code;
  uEchoString* name;
  uEchoString* desc;
} uEchoPropertyEnum, uEchoPropertyEnumList;

#define UECHO_PROPERTY_DATA_STRUCT_MEMBERS \
  uEchoString* name;                       \
  byte code;                               \
  int attr;                                \
  int type;                                \
  byte* data;                              \
  size_t data_size;

typedef struct {
  UECHO_LIST_STRUCT_MEMBERS
  UECHO_PROPERTY_DATA_STRUCT_MEMBERS
  void* parent_obj;
} uEchoProperty, uEchoPropertyList;

typedef struct {
  UECHO_PROPERTY_DATA_STRUCT_MEMBERS
} uEchoPropertyData;

/****************************************
 * Header
 ****************************************/

#include <uecho/property.h>

/****************************************
 * Function (Property)
 ****************************************/

bool uecho_property_setcount(uEchoProperty* prop, size_t count);
bool uecho_property_addcount(uEchoProperty* prop, size_t count);

bool uecho_property_announce(uEchoProperty* prop);

/****************************************
 * Function (Property List)
 ****************************************/

uEchoPropertyList* uecho_propertylist_new(void);
void uecho_propertylist_delete(uEchoPropertyList* props);

bool uecho_propertylist_add(uEchoPropertyList* props, uEchoProperty* prop);
bool uecho_propertylist_set(uEchoPropertyList* props, uEchoPropertyCode code, uEchoPropertyAttr attr, byte* data, size_t data_len);
bool uecho_propertylist_setdata(uEchoPropertyList* props, uEchoPropertyCode code, byte* data, size_t data_len);
bool uecho_propertylist_setintegerdata(uEchoPropertyList* props, uEchoPropertyCode code, int data, size_t data_len);
bool uecho_propertylist_setbytedata(uEchoPropertyList* props, uEchoPropertyCode code, byte data);
bool uecho_propertylist_setattribute(uEchoPropertyList* props, uEchoPropertyCode code, uEchoPropertyAttr attr);
uEchoProperty* uecho_propertylist_findbycode(uEchoPropertyList* props, uEchoPropertyCode code);

byte uecho_propertylist_getdatasize(uEchoPropertyList* props, uEchoPropertyCode code);
byte* uecho_propertylist_getdata(uEchoPropertyList* props, uEchoPropertyCode code);
bool uecho_propertylist_getintegerdata(uEchoPropertyList* props, uEchoPropertyCode code, int* data);
bool uecho_propertylist_getbytedata(uEchoPropertyList* props, uEchoPropertyCode code, byte* data);

#define uecho_propertylist_clear(props) uecho_list_clear((uEchoList*)props, (UECHO_LIST_DESTRUCTORFUNC)uecho_property_delete)
#define uecho_propertylist_size(props) uecho_list_size((uEchoList*)props)
#define uecho_propertylist_gets(props) (uEchoProperty*)uecho_list_next((uEchoList*)props)

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
