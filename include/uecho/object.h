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

#include <uecho/manufacture.h>
#include <uecho/message.h>
#include <uecho/property.h>
#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoObjectCodeMin = 0x000000,
  uEchoObjectCodeMax = 0xFFFFFF,
  uEchoObjectCodeUnknown = uEchoObjectCodeMin,
};

enum {
  uEchoNodeProfileObject = 0x0EF001,
  uEchoNodeProfileObjectReadOnly = 0x0EF002,
};

/****************************************
 * Data Type
 ****************************************/

typedef int uEchoObjectCode;

#if !defined(_UECHO_OBJECT_INTERNAL_H_)
typedef void uEchoObject;
#endif

typedef void (*uEchoObjectMessageListener)(uEchoObject*, uEchoMessage*);
typedef bool (*uEchoPropertyRequestHandler)(uEchoObject*, uEchoProperty*, uEchoEsv, size_t, byte*);

/****************************************
 * Function (Object)
 ****************************************/

uEchoObject* uecho_object_new(void);
bool uecho_object_delete(uEchoObject* obj);
uEchoObject* uecho_object_next(uEchoObject* obj);

void uecho_object_setname(uEchoObject* obj, const char* name);
const char* uecho_object_getname(uEchoObject* obj);

void uecho_object_setcode(uEchoObject* obj, uEchoObjectCode code);
uEchoObjectCode uecho_object_getcode(uEchoObject* obj);
bool uecho_object_iscode(uEchoObject* obj, uEchoObjectCode code);

bool uecho_object_start(uEchoObject* obj);
bool uecho_object_stop(uEchoObject* obj);
bool uecho_object_isrunning(uEchoObject* obj);

void uecho_object_setgroupcode(uEchoObject* obj, byte val);
byte uecho_object_getgroupcode(uEchoObject* obj);

void uecho_object_setclasscode(uEchoObject* obj, byte val);
byte uecho_object_getclasscode(uEchoObject* obj);

void uecho_object_setinstancecode(uEchoObject* obj, byte val);
byte uecho_object_getinstancecode(uEchoObject* obj);

bool uecho_object_isdevice(uEchoObject* obj);
bool uecho_object_isprofile(uEchoObject* obj);

bool uecho_object_addproperty(uEchoObject* obj, uEchoProperty* prop);
bool uecho_object_setproperty(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyAttr attr);
bool uecho_object_setpropertydata(uEchoObject* obj, uEchoPropertyCode code, byte* data, size_t dataLen);
bool uecho_object_setpropertyintegerdata(uEchoObject* obj, uEchoPropertyCode code, int data, size_t dataLen);
bool uecho_object_setpropertybytedata(uEchoObject* obj, uEchoPropertyCode code, byte data);
bool uecho_object_setpropertyattribute(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyAttr attr);

uEchoProperty* uecho_object_getproperties(uEchoObject* obj);
uEchoProperty* uecho_object_getproperty(uEchoObject* obj, uEchoPropertyCode code);
int uecho_object_getpropertydatasize(uEchoObject* obj, uEchoPropertyCode code);
byte* uecho_object_getpropertydata(uEchoObject* obj, uEchoPropertyCode code);
bool uecho_object_getpropertyintegerdata(uEchoObject* obj, uEchoPropertyCode code, int* data);
bool uecho_object_getpropertybytedata(uEchoObject* obj, uEchoPropertyCode code, byte* data);

bool uecho_object_hasproperty(uEchoObject* obj, uEchoPropertyCode code);
size_t uecho_object_getpropertycount(uEchoObject* obj);
void uecho_object_clearproperties(uEchoObject* obj);

void uecho_object_setmessagelistener(uEchoObject* obj, uEchoObjectMessageListener listener);
bool uecho_object_setpropertyrequesthandler(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestHandler handler);
bool uecho_object_setpropertywriterequesthandler(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyRequestHandler handler);
bool uecho_object_setpropertyreadhandler(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyRequestHandler handler);

bool uecho_object_announcemessage(uEchoObject* obj, uEchoMessage* msg);
bool uecho_object_sendmessage(uEchoObject* obj, uEchoMessage* msg);

/****************************************
 * Object Super Class
 ****************************************/

typedef enum {
  uEchoObjectManufacturerCode = 0x8A,
  uEchoObjectAnnoPropertyMap = 0x9D,
  uEchoObjectSetPropertyMap = 0x9E,
  uEchoObjectGetPropertyMap = 0x9F,
} uEchoObjectEPC;

typedef enum {
  uEchoObjectManufacturerCodeLen = 3,
  uEchoObjectPropertyMapMaxLen = 16,
  uEchoObjectAnnoPropertyMapMaxLen = (uEchoObjectPropertyMapMaxLen + 1),
  uEchoObjectSetPropertyMapMaxLen = (uEchoObjectPropertyMapMaxLen + 1),
  uEchoObjectGetPropertyMapMaxLen = (uEchoObjectPropertyMapMaxLen + 1),
} uEchoObjectEPCSize;

enum {
  uEchoManufactureCodeDefault = uEchoManufactureCodeTestMin,
};

bool uecho_object_setmanufacturercode(uEchoObject* obj, uEchoManufactureCode code);
uEchoManufactureCode uecho_object_getmanufacturercode(uEchoObject* obj);

/****************************************
 * Function (Property)
 ****************************************/

uEchoObject* uecho_property_getparentobject(uEchoProperty* prop);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_OBJECT_H_ */
