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
  byte statsByte;

  statsByte = stats ? uEchoNodeProfileClassBooting : uEchoNodeProfileClassNotBooting;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassOperatingStatus, &statsByte, uEchoNodeProfileClassOperatingStatusLen);
}

/****************************************
 * uecho_nodeprofileclass_setversion
 ****************************************/

bool uecho_nodeprofileclass_setversion(uEchoObject* obj, int majorVer, int minorVer)
{
  byte verBytes[uEchoNodeProfileClassVersionInformationLen];

  verBytes[0] = uEchoMajorVersion;
  verBytes[1] = uEchoMinorVersion;
  verBytes[2] = 0x01;
  verBytes[3] = 0x00;

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassVersionInformation, verBytes, uEchoNodeProfileClassVersionInformationLen);
}

/****************************************
 * uecho_nodeprofileclass_setid
 ****************************************/

bool uecho_nodeprofileclass_setid(uEchoObject* obj, byte* manCode, byte* uniqId)
{
  byte propData[uEchoNodeProfileClassIdentificationNumberLen];
  byte* prop;

  propData[0] = uEchoLowerCommunicationLayerProtocolType;

  prop = propData + 1;
  memcpy(prop, manCode, uEchoNodeProfileClassIdentificationManufacturerCodeLen);

  prop += uEchoNodeProfileClassIdentificationManufacturerCodeLen;
  memcpy(prop, uniqId, uEchoNodeProfileClassIdentificationUniqueIdLen);

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassIdentificationNumber, propData, uEchoNodeProfileClassIdentificationNumberLen);
}

/****************************************
 * uecho_nodeprofileclass_setdefaultid
 ****************************************/

bool uecho_nodeprofileclass_setdefaultid(uEchoObject* obj)
{
  byte manCode[uEchoNodeProfileClassIdentificationManufacturerCodeLen];
  byte uniqId[uEchoNodeProfileClassIdentificationUniqueIdLen];

  memset(manCode, 0, sizeof(manCode));
  memset(uniqId, 0, sizeof(uniqId));

  return uecho_nodeprofileclass_setid(obj, manCode, uniqId);
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

bool uecho_nodeprofileclass_setclasslist(uEchoObject* obj, int listCnt, byte* listBytes)
{
  if (uEchoNodeProfileClassSelfNodeClassListSMax < listCnt) {
    listCnt = uEchoNodeProfileClassSelfNodeClassListSMax;
  }
  listBytes[0] = listCnt;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassSelfNodeClassListS, listBytes, ((listCnt * 2) + 1));
}

/****************************************
 * uecho_nodeprofileclass_setinstancelist
 ****************************************/

bool uecho_nodeprofileclass_setinstancelist(uEchoObject* obj, int listCnt, byte* listBytes)
{
  bool isSuccess;

  if (uEchoNodeProfileClassSelfNodeInstanceListSMax < listCnt) {
    listCnt = uEchoNodeProfileClassSelfNodeInstanceListSMax;
  }
  listBytes[0] = listCnt;

  isSuccess = true;
  isSuccess &= uecho_object_setpropertydata(obj, uEchoNodeProfileClassSelfNodeInstanceListS, listBytes, ((listCnt * 3) + 1));
  isSuccess &= uecho_object_setpropertydata(obj, uEchoNodeProfileClassInstanceListNotification, listBytes, ((listCnt * 3) + 1));

  return isSuccess;
}

/****************************************
 * uecho_nodeprofileclass_isoperatingstatus
 ****************************************/

bool uecho_nodeprofileclass_isoperatingstatus(uEchoObject* obj)
{
  byte statsByte;

  if (!uecho_object_getpropertybytedata(obj, uEchoNodeProfileClassOperatingStatus, &statsByte))
    return false;

  return (statsByte == uEchoNodeProfileClassBooting) ? true : false;
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
  uEchoClass* nodeCls;
  uEchoObject* nodeObj;
  byte *nodeClassList, *nodeInstanceList;
  int nodeClassListCnt, nodeInstanceListCnt;
  int nodeClassCnt, nodeInstanceCnt;
  int idx;

  if (!obj)
    return false;

  node = uecho_object_getparentnode(obj);
  if (!node)
    return false;

  // Class Properties

  nodeClassList = (byte*)realloc(NULL, 1);
  nodeClassListCnt = 0;
  nodeClassCnt = 0;

  for (nodeCls = uecho_node_getclasses(node); nodeCls; nodeCls = uecho_class_next(nodeCls)) {
    nodeClassCnt++;

    if (uecho_class_isprofile(nodeCls))
      continue;

    nodeClassListCnt++;
    nodeClassList = (byte*)realloc(nodeClassList, ((2 * nodeClassListCnt) + 1));
    idx = (2 * (nodeClassListCnt - 1)) + 1;
    nodeClassList[idx + 0] = uecho_class_getclassgroupcode(nodeCls);
    nodeClassList[idx + 1] = uecho_class_getclasscode(nodeCls);
  }

  uecho_nodeprofileclass_setclasscount(obj, nodeClassCnt);
  uecho_nodeprofileclass_setclasslist(obj, nodeClassListCnt, nodeClassList);

  free(nodeClassList);

  // Instance Properties

  nodeInstanceList = (byte*)realloc(NULL, 1);
  nodeInstanceListCnt = 0;
  nodeInstanceCnt = 0;

  for (nodeObj = uecho_node_getobjects(node); nodeObj; nodeObj = uecho_object_next(nodeObj)) {
    if (uecho_object_isprofile(nodeObj))
      continue;

    nodeInstanceCnt++;

    nodeInstanceListCnt++;
    nodeInstanceList = (byte*)realloc(nodeInstanceList, ((3 * nodeInstanceListCnt) + 1));
    idx = (3 * (nodeInstanceListCnt - 1)) + 1;
    nodeInstanceList[idx + 0] = uecho_object_getclassgroupcode(nodeObj);
    nodeInstanceList[idx + 1] = uecho_object_getclasscode(nodeObj);
    nodeInstanceList[idx + 2] = uecho_object_getinstancecode(nodeObj);
  }

  uecho_nodeprofileclass_setinstancecount(obj, nodeInstanceCnt);
  uecho_nodeprofileclass_setinstancelist(obj, nodeInstanceListCnt, nodeInstanceList);

  free(nodeInstanceList);

  return true;
}
