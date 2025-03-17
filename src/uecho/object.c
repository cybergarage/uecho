/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_node.h>
#include <uecho/_object.h>

#include <uecho/controller.h>
#include <uecho/frame/observer.h>
#include <uecho/misc.h>
#include <uecho/node.h>
#include <uecho/profile.h>
#include <uecho/std/database.h>
#include <uecho/util/timer.h>

/****************************************
 * uecho_object_new
 ****************************************/

uEchoObject* uecho_object_new(void)
{
  uEchoObject* obj;

  obj = (uEchoObject*)malloc(sizeof(uEchoObject));

  if (!obj)
    return NULL;

  uecho_list_node_init((uEchoList*)obj);

  obj->name = uecho_string_new();

  uecho_object_setparentnode(obj, NULL);

  obj->code[0] = 0;
  obj->code[1] = 0;
  obj->code[2] = 0;

  obj->properties = uecho_propertylist_new();

  uecho_object_setmessagelistener(obj, NULL);
  obj->propListenerMgr = uecho_object_property_observer_manager_new();

  // Property map caches

  obj->annoPropMapSize = 0;
  obj->annoPropMapBytes = NULL;

  obj->setPropMapSize = 0;
  obj->setPropMapBytes = NULL;

  obj->getPropMapSize = 0;
  obj->getPropMapBytes = NULL;

  // Mandatory Properties

  if (!obj->name || !obj->properties || !obj->propListenerMgr) {
    uecho_object_delete(obj);
    return NULL;
  }

  return obj;
}

/****************************************
 * uecho_object_delete
 ****************************************/

bool uecho_object_delete(uEchoObject* obj)
{
  if (!obj)
    return false;

  uecho_list_remove((uEchoList*)obj);

  if (obj->name) {
    uecho_string_delete(obj->name);
  }

  uecho_object_clearpropertymapcaches(obj);

  if (obj->properties) {
    uecho_propertylist_delete(obj->properties);
  }

  if (obj->propListenerMgr) {
    uecho_object_property_observer_manager_delete(obj->propListenerMgr);
  }

  free(obj);

  return true;
}

/****************************************
 * uecho_object_next
 ****************************************/

uEchoObject* uecho_object_next(uEchoObject* obj)
{
  return (uEchoObject*)uecho_list_next((uEchoList*)obj);
}

/****************************************
 * uecho_object_setparentnode
 ****************************************/

bool uecho_object_setparentnode(uEchoObject* obj, uEchoNode* node)
{
  if (!obj)
    return false;

  obj->parentNode = node;

  return true;
}

/****************************************
 * uecho_object_getparentnode
 ****************************************/

uEchoNode* uecho_object_getparentnode(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return (uEchoNode*)obj->parentNode;
}

/****************************************
 * uecho_object_setname
 ****************************************/

void uecho_object_setname(uEchoObject* obj, const char* name)
{
  if (!obj)
    return;

  uecho_string_setvalue(obj->name, name);
}

/****************************************
 * uecho_object_getname
 ****************************************/

const char* uecho_object_getname(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return uecho_string_getvalue(obj->name);
}

/****************************************
 * uecho_object_addstandardproperties
 ****************************************/

bool uecho_object_addstandardpropertieswithcode(uEchoObject* obj, byte grpCode, byte clsCode)
{
  uEchoDatabase* db;
  uEchoObject* stdObj;

  if (!obj)
    return false;

  db = uecho_standard_getdatabase();
  if (!db)
    return false;

  stdObj = uecho_database_getobject(db, grpCode, clsCode);
  if (!stdObj)
    return false;

  uecho_object_setname(obj, uecho_object_getname(stdObj));

  if (!uecho_object_addmissingobjectproperties(obj, stdObj))
    return false;

  return true;
}

bool uecho_object_addstandardproperties(uEchoObject* obj)
{
  bool isSuccess = true;
  if (!uecho_object_addstandardpropertieswithcode(obj, 0x00, 0x00)) {
    isSuccess = false;
  }
  if (!uecho_object_addstandardpropertieswithcode(obj, uecho_object_getgroupcode(obj), uecho_object_getclasscode(obj))) {
    isSuccess = false;
  }
  return isSuccess;
}

/****************************************
 * uecho_object_setcode
 ****************************************/

void uecho_object_setcode(uEchoObject* obj, uEchoObjectCode val)
{
  if (!obj)
    return;

  obj->code[0] = (val & 0xFF0000) >> 16;
  obj->code[1] = (val & 0x00FF00) >> 8;
  obj->code[2] = (val & 0x0000FF);

  uecho_object_addstandardproperties(obj);
  uecho_object_updatepropertymaps(obj);
}

/****************************************
 * uecho_object_getcode
 ****************************************/

uEchoObjectCode uecho_object_getcode(uEchoObject* obj)
{
  int code;

  if (!obj)
    return 0;

  code = 0;
  code |= (obj->code[0] << 16) & 0xFF0000;
  code |= (obj->code[1] << 8) & 0x00FF00;
  code |= (obj->code[2]) & 0x0000FF;

  return code;
}

/****************************************
 * uecho_object_getcode
 ****************************************/

bool uecho_object_iscode(uEchoObject* obj, uEchoObjectCode code)
{
  return (uecho_object_getcode(obj) == code) ? true : false;
}

/****************************************
 * uecho_object_setgroupcode
 ****************************************/

void uecho_object_setgroupcode(uEchoObject* obj, byte val)
{
  if (!obj)
    return;

  obj->code[0] = val;

  uecho_object_addstandardproperties(obj);
}

/****************************************
 * uecho_object_getgroupcode
 ****************************************/

byte uecho_object_getgroupcode(uEchoObject* obj)
{
  if (!obj)
    return 0;

  return obj->code[0];
}

/****************************************
 * uecho_object_setclasscode
 ****************************************/

void uecho_object_setclasscode(uEchoObject* obj, byte val)
{
  if (!obj)
    return;

  obj->code[1] = val;

  uecho_object_addstandardproperties(obj);
}

/****************************************
 * uecho_object_getclasscode
 ****************************************/

byte uecho_object_getclasscode(uEchoObject* obj)
{
  if (!obj)
    return 0;

  return obj->code[1];
}

/****************************************
 * uecho_object_setinstancecode
 ****************************************/

void uecho_object_setinstancecode(uEchoObject* obj, byte val)
{
  if (!obj)
    return;

  obj->code[2] = val;
}

/****************************************
 * uecho_object_getinstancecode
 ****************************************/

byte uecho_object_getinstancecode(uEchoObject* obj)
{
  if (!obj)
    return 0;

  return obj->code[2];
}

/****************************************
 * uecho_object_isdevice
 ****************************************/

bool uecho_object_isdevice(uEchoObject* obj)
{
  if (!obj)
    return false;

  return uecho_isdeviceclassgroupcode(obj->code[0]);
}

/****************************************
 * uecho_object_isprofile
 ****************************************/

bool uecho_object_isprofile(uEchoObject* obj)
{
  if (!obj)
    return false;

  return uecho_isprofileclassgroupcode(obj->code[0]);
}

/****************************************
 * uecho_object_setpropertymap
 ****************************************/

bool uecho_propertymap_codetoformat2(uEchoPropertyCode propCode, int* propMapRow, int* propMapBit)
{
  if ((propCode < uEchoPropertyCodeMin) || (uEchoPropertyCodeMax < propCode))
    return false;
  // 1 <= prop_map_row <= 16
  *propMapRow = ((propCode - uEchoPropertyCodeMin) & 0x0F) + 1;
  // 0 <= prop_map_bit <= 7
  *propMapBit = (((propCode - uEchoPropertyCodeMin) & 0xF0) >> 4) & 0x0F;
  return true;
}

bool uecho_object_setpropertymap(uEchoObject* obj, uEchoPropertyCode mapCode, uEchoPropertyCode* propCodes, size_t propsCodeSize)
{
  byte propMapData[uEchoPropertyMapFormatMaxSize];
  uEchoPropertyCode propCode;
  int propMapRow, propMapBit;
  size_t n;

  if (!obj)
    return false;

  propMapData[0] = (byte)propsCodeSize;

  // Description Format 1

  if (propsCodeSize <= uEchoPropertyMapFormat1MaxSize) {
    memcpy((propMapData + 1), propCodes, propsCodeSize);
    uecho_propertylist_set(obj->properties, mapCode, uEchoPropertyAttrRead, propMapData, (propsCodeSize + 1));
    return true;
  }

  // Description Format 2

  memset((propMapData + 1), 0, uEchoPropertyMapFormat2MapSize);
  for (n = 0; n < propsCodeSize; n++) {
    propCode = propCodes[n];
    if (!uecho_propertymap_codetoformat2(propCode, &propMapRow, &propMapBit)) {
      continue;
    }
    propMapData[propMapRow] |= ((0x01 << propMapBit) & 0x0F);
  }

  uecho_propertylist_setdata(obj->properties, mapCode, propMapData, uEchoPropertyMapFormat2Size);

  return true;
}

/****************************************
 * uecho_object_addproperty
 ****************************************/

bool uecho_object_addproperty(uEchoObject* obj, uEchoProperty* prop)
{
  if (!obj || !prop)
    return false;

  if (!uecho_property_setparentobject(prop, obj))
    return false;

  return uecho_propertylist_add(obj->properties, prop);
}

/****************************************
 * uecho_object_setproperty
 ****************************************/

bool uecho_object_setproperty(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyAttr attr)
{
  uEchoProperty* prop;

  if (!obj)
    return false;

  if (!uecho_propertylist_set(obj->properties, code, attr, NULL, 0))
    return false;

  prop = uecho_object_getproperty(obj, code);
  if (!prop)
    return false;

  if (!uecho_property_setparentobject(prop, obj))
    return false;

  return uecho_object_updatepropertymaps(obj);
}

/****************************************
 * uecho_object_setpropertydata
 ****************************************/

bool uecho_object_setpropertydata(uEchoObject* obj, uEchoPropertyCode code, byte* data, size_t dataLen)
{
  if (!obj)
    return false;

  return uecho_propertylist_setdata(obj->properties, code, data, dataLen);
}

/****************************************
 * uecho_object_setpropertyintegerdata
 ****************************************/

bool uecho_object_setpropertyintegerdata(uEchoObject* obj, uEchoPropertyCode code, int data, size_t dataLen)
{
  if (!obj)
    return false;

  return uecho_propertylist_setintegerdata(obj->properties, code, data, dataLen);
}

/****************************************
 * uecho_object_setpropertybytedata
 ****************************************/

bool uecho_object_setpropertybytedata(uEchoObject* obj, uEchoPropertyCode code, byte data)
{
  if (!obj)
    return false;

  return uecho_propertylist_setbytedata(obj->properties, code, data);
}

/****************************************
 * uecho_object_setpropertyattribute
 ****************************************/

bool uecho_object_setpropertyattribute(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyAttr attr)
{
  if (!obj)
    return false;

  return uecho_propertylist_setattribute(obj->properties, code, attr);
}

/****************************************
 * uecho_object_getproperties
 ****************************************/

uEchoProperty* uecho_object_getproperties(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return uecho_propertylist_gets(obj->properties);
}

/****************************************
 * uecho_object_hasproperty
 ****************************************/

bool uecho_object_hasproperty(uEchoObject* obj, uEchoPropertyCode code)
{
  if (!obj)
    return false;

  return (uecho_object_getproperty(obj, code) != NULL) ? true : false;
}

/****************************************
 * uecho_object_getproperty
 ****************************************/

uEchoProperty* uecho_object_getproperty(uEchoObject* obj, uEchoPropertyCode code)
{
  if (!obj)
    return NULL;

  return uecho_propertylist_findbycode(obj->properties, code);
}

/****************************************
 * uecho_object_getpropertywait
 ****************************************/

uEchoProperty* uecho_object_getpropertywait(uEchoObject* obj, uEchoPropertyCode code, clock_t waitMiliTime)
{
  uEchoProperty* prop;
  int n;

  for (n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(waitMiliTime / uEchoWaitRetryCount);
    prop = uecho_object_getproperty(obj, code);
    if (prop)
      return prop;
  }

  return NULL;
}

/****************************************
 * uecho_object_getpropertycount
 ****************************************/

size_t uecho_object_getpropertycount(uEchoObject* obj)
{
  if (!obj)
    return 0;

  return uecho_propertylist_size(obj->properties);
}

/****************************************
 * uecho_object_clearproperties
 ****************************************/

void uecho_object_clearproperties(uEchoObject* obj)
{
  if (!obj)
    return;

  uecho_propertylist_clear(obj->properties);
  uecho_object_clearpropertymapcaches(obj);
}

/****************************************
 * uecho_object_getpropertydatasize
 ****************************************/

int uecho_object_getpropertydatasize(uEchoObject* obj, uEchoPropertyCode code)
{
  if (!obj)
    return 0;

  return uecho_propertylist_getdatasize(obj->properties, code);
}

/****************************************
 * uecho_object_getpropertydata
 ****************************************/

byte* uecho_object_getpropertydata(uEchoObject* obj, uEchoPropertyCode code)
{
  if (!obj)
    return NULL;

  return uecho_propertylist_getdata(obj->properties, code);
}

/****************************************
 * uecho_object_getpropertyintegerdata
 ****************************************/

bool uecho_object_getpropertyintegerdata(uEchoObject* obj, uEchoPropertyCode code, int* data)
{
  if (!obj)
    return false;

  return uecho_propertylist_getintegerdata(obj->properties, code, data);
}

/****************************************
 * uecho_object_getpropertybytedata
 ****************************************/

bool uecho_object_getpropertybytedata(uEchoObject* obj, uEchoPropertyCode code, byte* data)
{
  if (!obj)
    return false;

  return uecho_propertylist_getbytedata(obj->properties, code, data);
}

/****************************************
 * uecho_object_setmessagelistener
 ****************************************/

void uecho_object_setmessagelistener(uEchoObject* obj, uEchoObjectMessageListener listener)
{
  if (!obj)
    return;

  obj->allMsgListener = listener;
}

/****************************************
 * uecho_object_getmessagelistener
 ****************************************/

uEchoObjectMessageListener uecho_object_getmessagelistener(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return obj->allMsgListener;
}

/****************************************
 * uecho_object_hasmessagelistener
 ****************************************/

bool uecho_object_hasmessagelistener(uEchoObject* obj)
{
  if (!obj)
    return false;

  return obj->allMsgListener ? true : false;
}

/****************************************
 * uecho_object_setpropertyrequesthandler
 ****************************************/

bool uecho_object_setpropertyrequesthandler(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestHandler handler)
{
  if (!obj)
    return false;

  return uecho_object_property_observer_manager_setobserver(obj->propListenerMgr, esv, code, handler);
}

/****************************************
 * uecho_object_getpropertyrequestlistener
 ****************************************/

uEchoPropertyRequestHandler uecho_object_getpropertyrequestlistener(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code)
{
  uEchoObjectPropertyObserver* obs;

  if (!obj)
    return NULL;

  obs = uecho_object_property_observer_manager_getobserver(obj->propListenerMgr, esv, code);
  if (!obs)
    return NULL;

  return obs->handler;
}

/****************************************
 * uecho_object_haspropertyrequestlistener
 ****************************************/

bool uecho_object_haspropertyrequestlistener(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code)
{
  return (uecho_object_getpropertyrequestlistener(obj, esv, code) != NULL) ? true : false;
}

/****************************************
 * uecho_object_setpropertywriterequesthandler
 ****************************************/

bool uecho_object_setpropertywriterequesthandler(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyRequestHandler handler)
{
  bool isSeccess = true;

  isSeccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteRequest, code, handler);
  isSeccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteRequestResponseRequired, code, handler);
  isSeccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteReadRequest, code, handler);

  return isSeccess;
}

/****************************************
 * uecho_object_setpropertyreadhandler
 ****************************************/

bool uecho_object_setpropertyreadhandler(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyRequestHandler handler)
{
  bool isSeccess = true;

  isSeccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvReadRequest, code, handler);
  isSeccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvNotificationRequest, code, handler);
  isSeccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteReadRequest, code, handler);

  return isSeccess;
}

/****************************************
 * uecho_object_announcemessage
 ****************************************/

bool uecho_object_announcemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoNode* parentNode;

  if (!obj)
    return false;

  parentNode = uecho_object_getparentnode(obj);
  if (!parentNode)
    return false;

  uecho_message_setsourceobjectcode(msg, uecho_object_getcode(obj));

  return uecho_node_announcemessage(parentNode, msg);
}

/****************************************
 * uecho_object_sendmessage
 ****************************************/

bool uecho_object_sendmessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoNode* objNode;
  uEchoController* ctrl;

  if (!obj || !msg)
    return false;

  objNode = uecho_object_getparentnode(obj);
  if (!objNode)
    return false;

  ctrl = uecho_node_getcontroller(objNode);
  if (!ctrl)
    return false;

  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(obj));

  return uecho_controller_sendmessage(ctrl, objNode, msg);
}

/****************************************
 * uecho_object_addmissingobjectproperties
 ****************************************/

bool uecho_object_addmissingobjectproperties(uEchoObject* obj, uEchoObject* srcObj)
{
  uEchoProperty *srcProp, *copyProp;

  if (!obj)
    return false;

  for (srcProp = uecho_object_getproperties(srcObj); srcProp; srcProp = uecho_property_next(srcProp)) {
    if (uecho_object_hasproperty(obj, uecho_property_getcode(srcProp)))
      continue;
    copyProp = uecho_property_copy(srcProp);
    if (!copyProp)
      return false;
    uecho_object_addproperty(obj, copyProp);
  }

  return true;
}
