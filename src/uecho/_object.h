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

#include <uecho/_const.h>
#include <uecho/_message.h>
#include <uecho/_property.h>

#include <uecho/util/list.h>
#include <uecho/util/mutex.h>
#include <uecho/util/strings.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoObject {
  UECHO_LIST_STRUCT_MEMBERS

  uEchoString* name;
  byte code[3];
  uEchoPropertyList* properties;

  void* parentNode;

  // Property map caches

  size_t annoPropMapSize;
  uEchoPropertyCode* annoPropMapBytes;

  size_t setPropMapSize;
  uEchoPropertyCode* setPropMapBytes;

  size_t getPropMapSize;
  uEchoPropertyCode* getPropMapBytes;

  // Listener

  void (*allMsgListener)(struct UEchoObject*, uEchoMessage*); /* uEchoObjectMessageListener */
  void* propListenerMgr;
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

bool uecho_object_updatepropertymaps(uEchoObject* obj);
void uecho_object_clearpropertymapcaches(uEchoObject* obj);

uEchoProperty* uecho_object_getpropertywait(uEchoObject* obj, uEchoPropertyCode code, clock_t waitMiliTime);

bool uecho_object_setpropertymap(uEchoObject* obj, uEchoPropertyCode mapCode, uEchoPropertyCode* propCodes, size_t propsCodeSize);

bool uecho_object_addmissingobjectproperties(uEchoObject* obj, uEchoObject* srcObj);

/****************************************
 * Function (Object List)
 ****************************************/

uEchoObjectList* uecho_objectlist_new(void);
void uecho_objectlist_delete(uEchoObjectList* objs);

bool uecho_objectlist_set(uEchoObjectList* objs, uEchoObjectCode code);
uEchoObject* uecho_objectlist_getbycode(uEchoObjectList* objs, uEchoObjectCode code);
uEchoObject* uecho_objectlist_getbygroupclasscode(uEchoObjectList* objs, byte grpCode, byte clsCode);

#define uecho_objectlist_clear(objs) uecho_list_clear((uEchoList*)objs, (UECHO_LIST_DESTRUCTORFUNC)uecho_object_delete)
#define uecho_objectlist_size(objs) uecho_list_size((uEchoList*)objs)
#define uecho_objectlist_gets(objs) (uEchoObject*)uecho_list_next((uEchoList*)objs)
#define uecho_objectlist_add(objs, obj) uecho_list_add((uEchoList*)objs, (uEchoList*)obj)

/****************************************
 * Function (Property)
 ****************************************/

bool uecho_property_setparentobject(uEchoProperty* prop, uEchoObject* obj);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_OBJECT_INTERNAL_H_ */
