/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdlib.h>
#include <string.h>

#include <uecho/_object.h>
#include <uecho/const.h>
#include <uecho/misc.h>
#include <uecho/node.h>
#include <uecho/profile.h>
#include <uecho/std/database.h>

/****************************************
* uecho_property_new
****************************************/

uEchoObject* uecho_nodeprofile_new(void)
{
  uEchoObject* obj;

  obj = uecho_object_new();
  if (!obj)
    return NULL;

  uecho_object_setcode(obj, uEchoNodeProfileObject);
  uecho_nodeprofile_addmandatoryproperties(obj);

  return obj;
}

/****************************************
 * uecho_nodeprofile_addmandatoryproperties
 ****************************************/

bool uecho_nodeprofile_addmandatoryproperties(uEchoObject* obj)
{
  if (!obj)
    return false;

  uEchoDatabase* db;
  uEchoObject *super_obj;

  if (!obj)
    return false;

  db = uecho_standard_getdatabase();
  if (!db)
    return false;

  super_obj = uecho_database_getobject(db, 0x0E, 0xF0);
  if (!super_obj)
    return false;

  if (!uecho_object_copyobjectproperties(obj, super_obj))
    return false;
  
  uecho_nodeprofile_setoperatingstatus(obj, true);
  uecho_nodeprofile_setversion(obj, uEchoSupportedMajorVersion, uEchoSupportedMinorVersion);
  uecho_nodeprofile_setdefaultid(obj);

  return true;
}

/****************************************
 * uecho_nodeprofile_setoperatingstatus
 ****************************************/

bool uecho_nodeprofile_setoperatingstatus(uEchoObject* obj, bool stats)
{
  byte stats_byte;

  stats_byte = stats ? uEchoNodeProfileClassBooting : uEchoNodeProfileClassNotBooting;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassOperatingStatus, &stats_byte, uEchoNodeProfileClassOperatingStatusLen);
}

/****************************************
 * uecho_nodeprofile_setversion
 ****************************************/

bool uecho_nodeprofile_setversion(uEchoObject* obj, int major_ver, int minor_ver)
{
  byte ver_bytes[uEchoNodeProfileClassVersionInformationLen];

  ver_bytes[0] = major_ver;
  ver_bytes[1] = minor_ver;
  ver_bytes[2] = 0x00;
  ver_bytes[3] = 0x10;

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassVersionInformation, ver_bytes, uEchoNodeProfileClassVersionInformationLen);
}

/****************************************
 * uecho_nodeprofile_setid
 ****************************************/

bool uecho_nodeprofile_setid(uEchoObject* obj, byte* man_code, byte* uniq_id)
{
  byte prop_data[uEchoNodeProfileClassIdentificationNumberLen];
  byte* prop;

  prop_data[0] = uEchoLowerCommunicationLayerProtocolType;

  prop = prop_data + 1;
  memcpy(prop, man_code, uEchoNodeProfileClassIdentificationManufacturerCodeLen);

  prop += uEchoNodeProfileClassIdentificationManufacturerCodeLen;
  memcpy(prop, uniq_id, uEchoNodeProfileClassIdentificationUniqueIdLen);

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassIdentificationNumber, prop_data, uEchoNodeProfileClassIdentificationNumberLen);
}

/****************************************
 * uecho_nodeprofile_setdefaultid
 ****************************************/

bool uecho_nodeprofile_setdefaultid(uEchoObject* obj)
{
  byte man_code[uEchoNodeProfileClassIdentificationManufacturerCodeLen];
  byte uniq_id[uEchoNodeProfileClassIdentificationUniqueIdLen];
  size_t n;
  
  for (n=0; n<sizeof(man_code); n++) {
    man_code[n] = (byte)(rand() & 0xFF);
  }

  for (n=0; n<sizeof(uniq_id); n++) {
    uniq_id[n] = (byte)(rand() & 0xFF);
  }

  return uecho_nodeprofile_setid(obj, man_code, uniq_id);
}

/****************************************
 * uecho_nodeprofile_setclasscount
 ****************************************/

bool uecho_nodeprofile_setclasscount(uEchoObject* obj, int count)
{
  return uecho_object_setpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, count, uEchoNodeProfileClassNumberOfSelfNodeClassesLen);
}

/****************************************
 * uecho_nodeprofile_setinstancecount
 ****************************************/

bool uecho_nodeprofile_setinstancecount(uEchoObject* obj, int count)
{
  return uecho_object_setpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, count, uEchoNodeProfileClassNumberOfSelfNodeInstancesLen);
}

/****************************************
 * uecho_nodeprofile_setclasslist
 ****************************************/

bool uecho_nodeprofile_setclasslist(uEchoObject* obj, int list_cnt, byte* list_bytes)
{
  if (uEchoNodeProfileClassSelfNodeClassListSMax < list_cnt) {
    list_cnt = uEchoNodeProfileClassSelfNodeClassListSMax;
  }
  list_bytes[0] = list_cnt;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassSelfNodeClassListS, list_bytes, ((list_cnt * 2) + 1));
}

/****************************************
 * uecho_nodeprofile_setinstancelist
 ****************************************/

bool uecho_nodeprofile_setinstancelist(uEchoObject* obj, int list_cnt, byte* list_bytes)
{
  bool is_success;

  if (uEchoNodeProfileClassSelfNodeInstanceListSMax < list_cnt) {
    list_cnt = uEchoNodeProfileClassSelfNodeInstanceListSMax;
  }
  list_bytes[0] = list_cnt;

  is_success = true;
  is_success &= uecho_object_setpropertydata(obj, uEchoNodeProfileClassSelfNodeInstanceListS, list_bytes, ((list_cnt * 3) + 1));
  is_success &= uecho_object_setpropertydata(obj, uEchoNodeProfileClassInstanceListNotification, list_bytes, ((list_cnt * 3) + 1));

  return is_success;
}

/****************************************
 * uecho_nodeprofile_isoperatingstatus
 ****************************************/

bool uecho_nodeprofile_isoperatingstatus(uEchoObject* obj)
{
  byte stats_byte;

  if (!uecho_object_getpropertybytedata(obj, uEchoNodeProfileClassOperatingStatus, &stats_byte))
    return false;

  return (stats_byte == uEchoNodeProfileClassBooting) ? true : false;
}

/****************************************
 * uecho_nodeprofile_getinstancecount
 ****************************************/

int uecho_nodeprofile_getinstancecount(uEchoObject* obj)
{
  int count;

  if (!uecho_object_getpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, uEchoNodeProfileClassNumberOfSelfNodeInstancesLen, &count))
    return 0;

  return count;
}

/****************************************
 * uecho_nodeprofile_getclasscount
 ****************************************/

int uecho_nodeprofile_getclasscount(uEchoObject* obj)
{
  int count;

  if (!uecho_object_getpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, uEchoNodeProfileClassNumberOfSelfNodeClassesLen, &count))
    return 0;

  return count;
}

/****************************************
 * uecho_nodeprofile_getnotificationinstancelist
 ****************************************/

byte* uecho_nodeprofile_getnotificationinstancelist(uEchoObject* obj)
{
  return uecho_object_getpropertydata(obj, uEchoNodeProfileClassInstanceListNotification);
}

/****************************************
 * uecho_nodeprofile_getinstancelist
 ****************************************/

byte* uecho_nodeprofile_getinstancelist(uEchoObject* obj)
{
  return uecho_object_getpropertydata(obj, uEchoNodeProfileClassSelfNodeInstanceListS);
}

/****************************************
 * uecho_nodeprofile_getclasslist
 ****************************************/

byte* uecho_nodeprofile_getclasslist(uEchoObject* obj)
{
  return uecho_object_getpropertydata(obj, uEchoNodeProfileClassSelfNodeClassListS);
}

/****************************************
 * uecho_nodeprofile_updateinstanceproperties
 ****************************************/

bool uecho_nodeprofile_updateinstanceproperties(uEchoObject* obj)
{
  uEchoNode* node;
  uEchoClass* node_cls;
  uEchoObject* node_obj;
  byte *node_class_list, *node_instance_list;
  int node_class_list_cnt, node_instance_list_cnt;
  int node_class_cnt, node_instance_cnt;
  int idx;

  if (!obj)
    return false;

  node = uecho_object_getparentnode(obj);
  if (!node)
    return false;

  // Class Properties

  node_class_list = (byte*)realloc(NULL, 1);
  node_class_list_cnt = 0;
  node_class_cnt = 0;

  for (node_cls = uecho_node_getclasses(node); node_cls; node_cls = uecho_class_next(node_cls)) {
    node_class_cnt++;

    if (uecho_class_isprofile(node_cls))
      continue;

    node_class_list_cnt++;
    node_class_list = (byte*)realloc(node_class_list, ((2 * node_class_list_cnt) + 1));
    idx = (2 * (node_class_list_cnt - 1)) + 1;
    node_class_list[idx + 0] = uecho_class_getclassgroupcode(node_cls);
    node_class_list[idx + 1] = uecho_class_getclasscode(node_cls);
  }

  uecho_nodeprofile_setclasscount(obj, node_class_cnt);
  uecho_nodeprofile_setclasslist(obj, node_class_list_cnt, node_class_list);

  free(node_class_list);

  // Instance Properties

  node_instance_list = (byte*)realloc(NULL, 1);
  node_instance_list_cnt = 0;
  node_instance_cnt = 0;

  for (node_obj = uecho_node_getobjects(node); node_obj; node_obj = uecho_object_next(node_obj)) {
    if (uecho_object_isprofile(node_obj))
      continue;

    node_instance_cnt++;

    node_instance_list_cnt++;
    node_instance_list = (byte*)realloc(node_instance_list, ((3 * node_instance_list_cnt) + 1));
    idx = (3 * (node_instance_list_cnt - 1)) + 1;
    node_instance_list[idx + 0] = uecho_object_getgroupcode(node_obj);
    node_instance_list[idx + 1] = uecho_object_getclasscode(node_obj);
    node_instance_list[idx + 2] = uecho_object_getinstancecode(node_obj);
  }

  uecho_nodeprofile_setinstancecount(obj, node_instance_cnt);
  uecho_nodeprofile_setinstancelist(obj, node_instance_list_cnt, node_instance_list);

  free(node_instance_list);

  return true;
}
