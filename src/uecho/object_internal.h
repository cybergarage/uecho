/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_OBJECT_INTERNAL_H_
#define _UECHO_OBJECT_INTERNAL_H_

#include <uecho/typedef.h>

#include <uecho/const_internal.h>
#include <uecho/message_internal.h>
#include <uecho/property_internal.h>

#include <uecho/util/list.h>
#include <uecho/util/mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoObject {
  UECHO_LIST_STRUCT_MEMBERS

  byte code[3];
  uEchoPropertyList* properties;

  void* parent_node;

  // Property map caches

  size_t anno_prop_map_size;
  uEchoPropertyCode* anno_prop_map_bytes;

  size_t set_prop_map_size;
  uEchoPropertyCode* set_prop_map_bytes;

  size_t get_prop_map_size;
  uEchoPropertyCode* get_prop_map_bytes;

  // Listener

  void (*all_msg_listener)(struct UEchoObject*, uEchoMessage*); /* uEchoObjectMessageListener */
  void* prop_listener_mgr;
} uEchoObject, uEchoObjectList;

/****************************************
 * Header
 ****************************************/

#include <uecho/object.h>

/****************************************
 * Function
 ****************************************/

#define uecho_object_remove(obj) uecho_list_remove((uEchoList*)obj)

uEchoObjectMessageListener uecho_object_getmessagelistener(uEchoObject* obj);
bool uecho_object_hasmessagelistener(uEchoObject* obj);

uEchoPropertyRequestHandler uecho_object_getpropertyrequestlistener(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code);
bool uecho_object_haspropertyrequestlistener(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code);

bool uecho_object_addmandatoryproperties(uEchoObject* obj);
bool uecho_object_updatepropertymaps(uEchoObject* obj);
void uecho_object_clearpropertymapcaches(uEchoObject* obj);

uEchoProperty* uecho_object_getpropertywait(uEchoObject* obj, uEchoPropertyCode code, clock_t wait_mili_time);

bool uecho_object_setpropertymap(uEchoObject* obj, uEchoPropertyCode map_code, uEchoPropertyCode* prop_codes, size_t props_code_size);

/****************************************
 * Function (Object List)
 ****************************************/

uEchoObjectList* uecho_objectlist_new(void);
void uecho_objectlist_delete(uEchoObjectList* objs);

bool uecho_objectlist_set(uEchoObjectList* props, uEchoObjectCode code);
uEchoObject* uecho_objectlist_getbycode(uEchoObjectList* props, uEchoObjectCode code);

#define uecho_objectlist_clear(objs) uecho_list_clear((uEchoList*)objs, (UECHO_LIST_DESTRUCTORFUNC)uecho_object_delete)
#define uecho_objectlist_size(objs) uecho_list_size((uEchoList*)objs)
#define uecho_objectlist_gets(objs) (uEchoObject*)uecho_list_next((uEchoList*)objs)
#define uecho_objectlist_add(objs, obj) uecho_list_add((uEchoList*)objs, (uEchoList*)obj)

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_OBJECT_INTERNAL_H_ */
