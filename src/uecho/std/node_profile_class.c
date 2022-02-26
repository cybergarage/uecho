/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/const.h>
#include <uecho/misc.h>
#include <uecho/node.h>
#include <uecho/profile.h>

/****************************************
* uecho_property_new
****************************************/

uEchoObject* uecho_nodeprofileclass_new(void)
{
  uEchoObject* obj;

  obj = uecho_object_new();
  if (!obj)
    return NULL;

  uecho_object_setcode(obj, uEchoNodeProfileObject);
  uecho_nodeprofileclass_addmandatoryproperties(obj);

  return obj;
}

/****************************************
 * uecho_nodeprofileclass_addmandatoryproperties
 ****************************************/

bool uecho_nodeprofileclass_addmandatoryproperties(uEchoObject* obj)
{
  if (!obj)
    return false;

  // Operation Status

  uecho_object_setproperty(obj, uEchoNodeProfileClassOperatingStatus, uEchoPropertyAttrReadAnno);
  uecho_nodeprofileclass_setoperatingstatus(obj, true);

  // Version Information

  uecho_object_setproperty(obj, uEchoNodeProfileClassVersionInformation, uEchoPropertyAttrRead);
  uecho_nodeprofileclass_setversion(obj, uEchoMajorVersion, uEchoMinorVersion);

  // Identification Number

  uecho_object_setproperty(obj, uEchoNodeProfileClassIdentificationNumber, uEchoPropertyAttrRead);
  uecho_nodeprofileclass_setdefaultid(obj);

  // Number Of Self Node Instances

  uecho_object_setproperty(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, uEchoPropertyAttrRead);

  // Number Of Self Node Classes

  uecho_object_setproperty(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, uEchoPropertyAttrRead);

  // Instance List Notification

  uecho_object_setproperty(obj, uEchoNodeProfileClassInstanceListNotification, uEchoPropertyAttrAnno);

  // Self Node Instance ListS

  uecho_object_setproperty(obj, uEchoNodeProfileClassSelfNodeInstanceListS, uEchoPropertyAttrRead);

  // Self Node Class List S

  uecho_object_setproperty(obj, uEchoNodeProfileClassSelfNodeClassListS, uEchoPropertyAttrRead);

  return true;
}

/****************************************
 * uecho_nodeprofileclass_setoperatingstatus
 ****************************************/

bool uecho_nodeprofileclass_setoperatingstatus(uEchoObject* obj, bool stats)
{
  byte stats_byte;

  stats_byte = stats ? uEchoNodeProfileClassBooting : uEchoNodeProfileClassNotBooting;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassOperatingStatus, &stats_byte, uEchoNodeProfileClassOperatingStatusLen);
}

/****************************************
 * uecho_nodeprofileclass_setversion
 ****************************************/

bool uecho_nodeprofileclass_setversion(uEchoObject* obj, int major_ver, int minor_ver)
{
  byte ver_bytes[uEchoNodeProfileClassVersionInformationLen];

  ver_bytes[0] = uEchoMajorVersion;
  ver_bytes[1] = uEchoMinorVersion;
  ver_bytes[2] = 0x01;
  ver_bytes[3] = 0x00;

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassVersionInformation, ver_bytes, uEchoNodeProfileClassVersionInformationLen);
}

/****************************************
 * uecho_nodeprofileclass_setid
 ****************************************/

bool uecho_nodeprofileclass_setid(uEchoObject* obj, byte* man_code, byte* uniq_id)
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
 * uecho_nodeprofileclass_setdefaultid
 ****************************************/

bool uecho_nodeprofileclass_setdefaultid(uEchoObject* obj)
{
  byte man_code[uEchoNodeProfileClassIdentificationManufacturerCodeLen];
  byte uniq_id[uEchoNodeProfileClassIdentificationUniqueIdLen];

  memset(man_code, 0, sizeof(man_code));
  memset(uniq_id, 0, sizeof(uniq_id));

  return uecho_nodeprofileclass_setid(obj, man_code, uniq_id);
}

/****************************************
 * uecho_nodeprofileclass_setclasscount
 ****************************************/

bool uecho_nodeprofileclass_setclasscount(uEchoObject* obj, int count)
{
  return uecho_object_setpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, count, uEchoNodeProfileClassNumberOfSelfNodeClassesLen);
}

/****************************************
 * uecho_nodeprofileclass_setinstancecount
 ****************************************/

bool uecho_nodeprofileclass_setinstancecount(uEchoObject* obj, int count)
{
  return uecho_object_setpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, count, uEchoNodeProfileClassNumberOfSelfNodeInstancesLen);
}

/****************************************
 * uecho_nodeprofileclass_setclasslist
 ****************************************/

bool uecho_nodeprofileclass_setclasslist(uEchoObject* obj, int list_cnt, byte* list_bytes)
{
  if (uEchoNodeProfileClassSelfNodeClassListSMax < list_cnt) {
    list_cnt = uEchoNodeProfileClassSelfNodeClassListSMax;
  }
  list_bytes[0] = list_cnt;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassSelfNodeClassListS, list_bytes, ((list_cnt * 2) + 1));
}

/****************************************
 * uecho_nodeprofileclass_setinstancelist
 ****************************************/

bool uecho_nodeprofileclass_setinstancelist(uEchoObject* obj, int list_cnt, byte* list_bytes)
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
 * uecho_nodeprofileclass_isoperatingstatus
 ****************************************/

bool uecho_nodeprofileclass_isoperatingstatus(uEchoObject* obj)
{
  byte stats_byte;

  if (!uecho_object_getpropertybytedata(obj, uEchoNodeProfileClassOperatingStatus, &stats_byte))
    return false;

  return (stats_byte == uEchoNodeProfileClassBooting) ? true : false;
}

/****************************************
 * uecho_nodeprofileclass_getinstancecount
 ****************************************/

int uecho_nodeprofileclass_getinstancecount(uEchoObject* obj)
{
  int count;

  if (!uecho_object_getpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, uEchoNodeProfileClassNumberOfSelfNodeInstancesLen, &count))
    return 0;

  return count;
}

/****************************************
 * uecho_nodeprofileclass_getclasscount
 ****************************************/

int uecho_nodeprofileclass_getclasscount(uEchoObject* obj)
{
  int count;

  if (!uecho_object_getpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, uEchoNodeProfileClassNumberOfSelfNodeClassesLen, &count))
    return 0;

  return count;
}

/****************************************
 * uecho_nodeprofileclass_getnotificationinstancelist
 ****************************************/

byte* uecho_nodeprofileclass_getnotificationinstancelist(uEchoObject* obj)
{
  return uecho_object_getpropertydata(obj, uEchoNodeProfileClassInstanceListNotification);
}

/****************************************
 * uecho_nodeprofileclass_getinstancelist
 ****************************************/

byte* uecho_nodeprofileclass_getinstancelist(uEchoObject* obj)
{
  return uecho_object_getpropertydata(obj, uEchoNodeProfileClassSelfNodeInstanceListS);
}

/****************************************
 * uecho_nodeprofileclass_getclasslist
 ****************************************/

byte* uecho_nodeprofileclass_getclasslist(uEchoObject* obj)
{
  return uecho_object_getpropertydata(obj, uEchoNodeProfileClassSelfNodeClassListS);
}

/****************************************
 * uecho_nodeprofileclass_updateinstanceproperties
 ****************************************/

bool uecho_nodeprofileclass_updateinstanceproperties(uEchoObject* obj)
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

  uecho_nodeprofileclass_setclasscount(obj, node_class_cnt);
  uecho_nodeprofileclass_setclasslist(obj, node_class_list_cnt, node_class_list);

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
    node_instance_list[idx + 0] = uecho_object_getclassgroupcode(node_obj);
    node_instance_list[idx + 1] = uecho_object_getclasscode(node_obj);
    node_instance_list[idx + 2] = uecho_object_getinstancecode(node_obj);
  }

  uecho_nodeprofileclass_setinstancecount(obj, node_instance_cnt);
  uecho_nodeprofileclass_setinstancelist(obj, node_instance_list_cnt, node_instance_list);

  free(node_instance_list);

  return true;
}
