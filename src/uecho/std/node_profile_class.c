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

  obj = uecho_profile_new();
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
  uEchoObject* superObj;

  if (!obj)
    return false;

  db = uecho_standard_getdatabase();
  if (!db)
    return false;

  superObj = uecho_database_getobject(db, 0x0E, 0xF0);
  if (!superObj)
    return false;

  if (!uecho_object_addmissingobjectproperties(obj, superObj))
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
  byte statsByte;

  statsByte = stats ? uEchoNodeProfileClassBooting : uEchoNodeProfileClassNotBooting;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassOperatingStatus, &statsByte, uEchoNodeProfileClassOperatingStatusLen);
}

/****************************************
 * uecho_nodeprofile_setversion
 ****************************************/

bool uecho_nodeprofile_setversion(uEchoObject* obj, int majorVer, int minorVer)
{
  byte verBytes[uEchoNodeProfileClassVersionInformationLen];

  verBytes[0] = majorVer;
  verBytes[1] = minorVer;
  verBytes[2] = 0x00;
  verBytes[3] = 0x10;

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassVersionInformation, verBytes, uEchoNodeProfileClassVersionInformationLen);
}

/****************************************
 * uecho_nodeprofile_setid
 ****************************************/

bool uecho_nodeprofile_setid(uEchoObject* obj, byte* manCode, byte* uniqId)
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
 * uecho_nodeprofile_setdefaultid
 ****************************************/

bool uecho_nodeprofile_setdefaultid(uEchoObject* obj)
{
  byte manCode[uEchoNodeProfileClassIdentificationManufacturerCodeLen];
  byte uniqId[uEchoNodeProfileClassIdentificationUniqueIdLen];
  size_t n;

  for (n = 0; n < sizeof(manCode); n++) {
    manCode[n] = (byte)(rand() & 0xFF);
  }

  for (n = 0; n < sizeof(uniqId); n++) {
    uniqId[n] = (byte)(rand() & 0xFF);
  }

  return uecho_nodeprofile_setid(obj, manCode, uniqId);
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

bool uecho_nodeprofile_setclasslist(uEchoObject* obj, int listCnt, byte* listBytes)
{
  if (uEchoNodeProfileClassSelfNodeClassListSMax < listCnt) {
    listCnt = uEchoNodeProfileClassSelfNodeClassListSMax;
  }
  listBytes[0] = listCnt;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassSelfNodeClassListS, listBytes, ((listCnt * 2) + 1));
}

/****************************************
 * uecho_nodeprofile_setinstancelist
 ****************************************/

bool uecho_nodeprofile_setinstancelist(uEchoObject* obj, int listCnt, byte* listBytes)
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
 * uecho_nodeprofile_isoperatingstatus
 ****************************************/

bool uecho_nodeprofile_isoperatingstatus(uEchoObject* obj)
{
  byte statsByte;

  if (!uecho_object_getpropertybytedata(obj, uEchoNodeProfileClassOperatingStatus, &statsByte))
    return false;

  return (statsByte == uEchoNodeProfileClassBooting) ? true : false;
}

/****************************************
 * uecho_nodeprofile_getinstancecount
 ****************************************/

int uecho_nodeprofile_getinstancecount(uEchoObject* obj)
{
  int count;

  if (!uecho_object_getpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, &count))
    return 0;

  return count;
}

/****************************************
 * uecho_nodeprofile_getclasscount
 ****************************************/

int uecho_nodeprofile_getclasscount(uEchoObject* obj)
{
  int count;

  if (!uecho_object_getpropertyintegerdata(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, &count))
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

  uecho_nodeprofile_setclasscount(obj, nodeClassCnt);
  uecho_nodeprofile_setclasslist(obj, nodeClassListCnt, nodeClassList);

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
    nodeInstanceList[idx + 0] = uecho_object_getgroupcode(nodeObj);
    nodeInstanceList[idx + 1] = uecho_object_getclasscode(nodeObj);
    nodeInstanceList[idx + 2] = uecho_object_getinstancecode(nodeObj);
  }

  uecho_nodeprofile_setinstancecount(obj, nodeInstanceCnt);
  uecho_nodeprofile_setinstancelist(obj, nodeInstanceListCnt, nodeInstanceList);

  free(nodeInstanceList);

  return true;
}
