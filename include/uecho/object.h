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
#include <uecho/message.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoObjectCodeMin     = 0x000000,
  uEchoObjectCodeMax     = 0xFFFFFF,
  uEchoObjectCodeUnknown = uEchoObjectCodeMin,
};
  
enum {
  uEchoNodeProfileObject = 0x0EF001,
};
  
/****************************************
 * Data Type
 ****************************************/

typedef int uEchoManufacturerCode;
typedef int uEchoObjectCode;
  
typedef struct _uEchoObject {
  bool headFlag;
  struct _uEchoObject *prev;
  struct _uEchoObject *next;

  byte code[3];
  uEchoPropertyList *properties;

  void *parentNode;
  
  // Property map caches

  size_t annoPropMapSize;
  byte *annoPropMapBytes;
  
  size_t setPropMapSize;
  byte *setPropMapBytes;

  size_t getPropMapSize;
  byte *getPropMapBytes;

  // Listener
  
  void (*allMsgListener)(struct _uEchoObject *, uEchoMessage *); /* uEchoObjectMessageListener */
  void *propListenerMgr;
} uEchoObject, uEchoObjectList;

typedef void (*uEchoObjectMessageListener)(uEchoObject *, uEchoMessage *);
typedef void (*uEchoPropertyRequestListener)(uEchoObject *, uEchoEsv, uEchoProperty *);

/****************************************
 * Function (Object)
 ****************************************/

uEchoObject *uecho_object_new(void);
void uecho_object_delete(uEchoObject *obj);
#define uecho_object_next(obj) (uEchoObject *)uecho_list_next((uEchoList *)obj)
#define uecho_object_remove(obj) uecho_list_remove((uEchoList *)obj)

void uecho_object_setcode(uEchoObject *obj, uEchoObjectCode code);
uEchoObjectCode uecho_object_getcode(uEchoObject *obj);
bool uecho_object_iscode(uEchoObject *obj, uEchoObjectCode code);
  
bool uecho_object_start(uEchoObject *obj);
bool uecho_object_stop(uEchoObject *obj);
bool uecho_object_isrunning(uEchoObject *obj);

void uecho_object_setclassgroupcode(uEchoObject *obj, byte val);
byte uecho_object_getclassgroupcode(uEchoObject *obj);

void uecho_object_setclasscode(uEchoObject *obj, byte val);
byte uecho_object_getclasscode(uEchoObject *obj);

void uecho_object_setinstancecode(uEchoObject *obj, byte val);
byte uecho_object_getinstancecode(uEchoObject *obj);

bool uecho_object_isdevice(uEchoObject *obj);
bool uecho_object_isprofile(uEchoObject *obj);

bool uecho_object_setproperty(uEchoObject *obj, uEchoPropertyCode code, uEchoPropertyAttr attr);
bool uecho_object_setpropertydata(uEchoObject *obj, uEchoPropertyCode code, byte *data, size_t dataLen);
bool uecho_object_setpropertyintegerdata(uEchoObject *obj, uEchoPropertyCode code, int data, size_t dataLen);
bool uecho_object_setpropertybytedata(uEchoObject *obj, uEchoPropertyCode code, byte data);
bool uecho_object_setpropertyattribute(uEchoObject *obj, uEchoPropertyCode code, uEchoPropertyAttr attr);

uEchoProperty *uecho_object_getproperties(uEchoObject *obj);
uEchoProperty *uecho_object_getproperty(uEchoObject *obj, uEchoPropertyCode code);
int uecho_object_getpropertydatasize(uEchoObject *obj, uEchoPropertyCode code);
byte *uecho_object_getpropertydata(uEchoObject *obj, uEchoPropertyCode code);
bool uecho_object_getpropertyintegerdata(uEchoObject *obj, uEchoPropertyCode code, size_t dataLen, int *data);
bool uecho_object_getpropertybytedata(uEchoObject *obj, uEchoPropertyCode code, byte *data);

bool uecho_object_hasproperty(uEchoObject *obj, uEchoPropertyCode code);
size_t uecho_object_getpropertycount(uEchoObject *obj);
void uecho_object_clearproperties(uEchoObject *obj);

void uecho_object_setmessagelistener(uEchoObject *obj, uEchoObjectMessageListener listener);
uEchoObjectMessageListener uecho_object_getmessagelistener(uEchoObject *obj);
bool uecho_object_hasmessagelistener(uEchoObject *obj);

bool uecho_object_setpropertyrequestlistener(uEchoObject *obj, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestListener listener);
uEchoPropertyRequestListener uecho_object_getpropertyrequestlistener(uEchoObject *obj, uEchoEsv esv, uEchoPropertyCode code);
bool uecho_object_haspropertyrequestlistener(uEchoObject *obj, uEchoEsv esv, uEchoPropertyCode code);

bool uecho_object_announcemessage(uEchoObject *obj, uEchoMessage *msg);
bool uecho_object_sendmessage(uEchoObject *obj, uEchoObject *dstObj, uEchoMessage *msg);

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

enum {
  uEchoManufactureCodeUnknown = 0x000000,
};

bool uecho_object_addmandatoryproperties(uEchoObject *obj);
bool uecho_object_updatepropertymaps(uEchoObject *obj);
void uecho_object_clearpropertymapcaches(uEchoObject *obj);

bool uecho_object_setmanufacturercode(uEchoObject *obj, uEchoManufacturerCode code);
uEchoManufacturerCode uecho_object_getmanufacturercode(uEchoObject *obj);
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
