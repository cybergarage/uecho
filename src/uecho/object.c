/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_object.h>
#include <uecho/_node.h>

#include <uecho/frame/observer.h>
#include <uecho/misc.h>
#include <uecho/node.h>
#include <uecho/profile.h>
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

  uecho_object_setgroupcode(obj, 0);
  uecho_object_setclasscode(obj, 0);
  uecho_object_setinstancecode(obj, 0);

  obj->properties = uecho_propertylist_new();

  uecho_object_setmessagelistener(obj, NULL);
  obj->prop_listener_mgr = uecho_object_property_observer_manager_new();

  // Property map caches

  obj->anno_prop_map_size = 0;
  obj->anno_prop_map_bytes = NULL;

  obj->set_prop_map_size = 0;
  obj->set_prop_map_bytes = NULL;

  obj->get_prop_map_size = 0;
  obj->get_prop_map_bytes = NULL;

  // Mandatory Properties

  uecho_object_addmandatoryproperties(obj);

  if (!obj->name || !obj->properties || !obj->prop_listener_mgr) {
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

  if (obj->prop_listener_mgr) {
    uecho_object_property_observer_manager_delete(obj->prop_listener_mgr);
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

  obj->parent_node = node;

  return true;
}

/****************************************
 * uecho_object_getparentnode
 ****************************************/

uEchoNode* uecho_object_getparentnode(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return (uEchoNode*)obj->parent_node;
}

/****************************************
 * uecho_object_setname
 ****************************************/

void uecho_object_setname(uEchoObject* obj, const char *name)
{
  if (!obj)
    return;

  uecho_string_setvalue(obj->name, name);
}

/****************************************
 * uecho_object_getname
 ****************************************/

const char *uecho_object_getname(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return uecho_string_getvalue(obj->name);
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

bool uecho_object_setpropertymap(uEchoObject* obj, uEchoPropertyCode map_code, uEchoPropertyCode* prop_codes, size_t props_code_size)
{
  byte prop_map_data[uEchoPropertyMapFormatMaxSize];
  uEchoPropertyCode* prop_map;
  size_t n, prop_byte_idx;

  if (!obj)
    return false;

  prop_map_data[0] = (byte)props_code_size;
  prop_map = prop_map_data + 1;

  // Description Format 1

  if (props_code_size <= uEchoPropertyMapFormat1MaxSize) {
    memcpy(prop_map, prop_codes, props_code_size);
    uecho_propertylist_set(obj->properties, map_code, uEchoPropertyAttrRead, prop_map_data, (props_code_size + 1));
    return true;
  }

  // Description Format 2

  for (n = 0; n < props_code_size; n++) {
    byte prop_code;
    prop_code = prop_codes[n];
    if ((prop_code < uEchoPropertyCodeMin) || (uEchoPropertyCodeMax < prop_code))
      continue;
    prop_byte_idx = (prop_code - uEchoPropertyCodeMin) & 0x0F;
    prop_map[prop_byte_idx] |= (((prop_code - uEchoPropertyCodeMin) & 0xF0) >> 8) & 0x0F;
  }

  uecho_propertylist_set(obj->properties, map_code, uEchoPropertyAttrRead, prop_map_data, (props_code_size + 1));

  return true;
}

/****************************************
 * uecho_object_addproperty
 ****************************************/

bool uecho_object_addproperty(uEchoObject* obj, uEchoProperty *prop)
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

bool uecho_object_setpropertydata(uEchoObject* obj, uEchoPropertyCode code, byte* data, size_t data_len)
{
  if (!obj)
    return false;

  return uecho_propertylist_setdata(obj->properties, code, data, data_len);
}

/****************************************
 * uecho_object_setpropertyintegerdata
 ****************************************/

bool uecho_object_setpropertyintegerdata(uEchoObject* obj, uEchoPropertyCode code, int data, size_t data_len)
{
  if (!obj)
    return false;

  return uecho_propertylist_setintegerdata(obj->properties, code, data, data_len);
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

uEchoProperty* uecho_object_getpropertywait(uEchoObject* obj, uEchoPropertyCode code, clock_t wait_mili_time)
{
  uEchoProperty* prop;
  int n;

  for (n = 0; n < uEchoWaitRetryCount; n++) {
    uecho_sleep(wait_mili_time / uEchoWaitRetryCount);
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

bool uecho_object_getpropertyintegerdata(uEchoObject* obj, uEchoPropertyCode code, size_t data_len, int* data)
{
  if (!obj)
    return false;

  return uecho_propertylist_getintegerdata(obj->properties, code, data_len, data);
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

  obj->all_msg_listener = listener;
}

/****************************************
 * uecho_object_getmessagelistener
 ****************************************/

uEchoObjectMessageListener uecho_object_getmessagelistener(uEchoObject* obj)
{
  if (!obj)
    return NULL;

  return obj->all_msg_listener;
}

/****************************************
 * uecho_object_hasmessagelistener
 ****************************************/

bool uecho_object_hasmessagelistener(uEchoObject* obj)
{
  if (!obj)
    return false;

  return obj->all_msg_listener ? true : false;
}

/****************************************
 * uecho_object_setpropertyrequesthandler
 ****************************************/

bool uecho_object_setpropertyrequesthandler(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestHandler handler)
{
  if (!obj)
    return false;

  return uecho_object_property_observer_manager_setobserver(obj->prop_listener_mgr, esv, code, handler);
}

/****************************************
 * uecho_object_getpropertyrequestlistener
 ****************************************/

uEchoPropertyRequestHandler uecho_object_getpropertyrequestlistener(uEchoObject* obj, uEchoEsv esv, uEchoPropertyCode code)
{
  uEchoObjectPropertyObserver* obs;

  if (!obj)
    return NULL;

  obs = uecho_object_property_observer_manager_getobserver(obj->prop_listener_mgr, esv, code);
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
  bool is_seccess = true;

  is_seccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteRequest, code, handler);
  is_seccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteRequestResponseRequired, code, handler);
  is_seccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteReadRequest, code, handler);

  return is_seccess;
}

/****************************************
 * uecho_object_setpropertyreadhandler
 ****************************************/

bool uecho_object_setpropertyreadhandler(uEchoObject* obj, uEchoPropertyCode code, uEchoPropertyRequestHandler handler)
{
  bool is_seccess = true;

  is_seccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvReadRequest, code, handler);
  is_seccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvNotificationRequest, code, handler);
  is_seccess &= uecho_object_setpropertyrequesthandler(obj, uEchoEsvWriteReadRequest, code, handler);

  return is_seccess;
}

/****************************************
 * uecho_object_announcemessage
 ****************************************/

bool uecho_object_announcemessage(uEchoObject* obj, uEchoMessage* msg)
{
  uEchoNode* parent_node;

  if (!obj)
    return false;

  parent_node = uecho_object_getparentnode(obj);
  if (!parent_node)
    return false;

  uecho_message_setsourceobjectcode(msg, uecho_object_getcode(obj));

  return uecho_node_announcemessage(parent_node, msg);
}

/****************************************
 * uecho_object_sendmessage
 ****************************************/

bool uecho_object_sendmessage(uEchoObject* obj, uEchoObject* dst_obj, uEchoMessage* msg)
{
  uEchoNode *parent_node, *dst_parent_node;

  if (!obj || !dst_obj)
    return false;

  parent_node = uecho_object_getparentnode(obj);
  dst_parent_node = uecho_object_getparentnode(dst_obj);
  if (!parent_node || !dst_parent_node)
    return false;

  uecho_message_setsourceobjectcode(msg, uecho_object_getcode(obj));
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(dst_obj));

  return uecho_node_sendmessage(parent_node, dst_parent_node, msg);
}

/****************************************
 * uecho_object_copyobjectproperties
 ****************************************/

bool uecho_object_copyobjectproperties(uEchoObject* obj, uEchoObject* src_obj)
{
  uEchoProperty *src_prop, *copy_prop;

  if (!obj)
    return false;

  for (src_prop = uecho_object_getproperties(src_obj); src_prop; src_prop = uecho_property_next(src_prop)) {
    copy_prop = uecho_property_copy(src_prop);
    if (!copy_prop)
      return false;
    uecho_object_addproperty(obj, copy_prop);
  }

  return true;
}
