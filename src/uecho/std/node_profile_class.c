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
#include <uecho/node.h>
#include <uecho/profile.h>
#include <uecho/misc.h>

/****************************************
* uecho_property_new
****************************************/

uEchoObject *uecho_nodeprofileclass_new(void)
{
  uEchoObject *obj;
  
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

bool uecho_nodeprofileclass_addmandatoryproperties(uEchoObject *obj)
{
  // Operation Status
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassOperatingStatus, uEchoPropertyAttrReadAnno);
  uecho_nodeprofileclass_setoperatingstatus(obj, true);
  
  // Version Information
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassVersionInformation, uEchoPropertyAttrRead);
  uecho_nodeprofileclass_setversion(obj, uEchoMajorVersion, uEchoMinorVersion, uEchoSpecifiedMessageFormat);

  // Identification Number
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassIdentificationNumber, uEchoPropertyAttrRead);
  uecho_nodeprofileclass_setdefaultid(obj);
  
  // Number Of Self Node Instances

  uecho_object_addproperty(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, uEchoPropertyAttrRead);

  // Number Of Self Node Classes
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses, uEchoPropertyAttrRead);

  // Instance List Notification
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassInstanceListNotification, uEchoPropertyAttrAnno);

  // Self Node Instance ListS
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassSelfNodeInstanceListS, uEchoPropertyAttrRead);

  // Self Node Class List S
  
  uecho_object_addproperty(obj, uEchoNodeProfileClassSelfNodeClassListS, uEchoPropertyAttrRead);
  
  return true;
}

/****************************************
 * uecho_nodeprofileclass_setoperatingstatus
 ****************************************/

bool uecho_nodeprofileclass_setoperatingstatus(uEchoObject *obj, bool stats)
{
  byte statsByte;
  
  statsByte = stats ? uEchoNodeProfileClassBooting : uEchoNodeProfileClassNotBooting;
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassOperatingStatus, &statsByte, uEchoNodeProfileClassOperatingStatusLen);
}

/****************************************
 * uecho_nodeprofileclass_setversion
 ****************************************/

bool uecho_nodeprofileclass_setversion(uEchoObject *obj, int majorVer, int minorVer, uEchoMessageFormatType msgType)
{
  byte verBytes[uEchoNodeProfileClassVersionInformationLen];
  
  verBytes[0] = uEchoMajorVersion;
  verBytes[1] = uEchoMinorVersion;
  verBytes[2] = uEchoSpecifiedMessageFormat;
  verBytes[3] = 0x00;

  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassVersionInformation, verBytes, uEchoNodeProfileClassVersionInformationLen);
}

/****************************************
 * uecho_nodeprofileclass_setid
 ****************************************/

bool uecho_nodeprofileclass_setid(uEchoObject *obj, byte *manCode, byte *uniqId)
{
  byte propData[uEchoNodeProfileClassIdentificationNumberLen];
  byte *prop;
  
  propData[0] = uEchoLowerCommunicationLayerProtocolType;

  prop = propData + 1;
  memcpy(prop, manCode, uEchoNodeProfileClassIdentificationManufacturerCodeLen);
  
  prop += uEchoNodeProfileClassIdentificationManufacturerCodeLen;
  memcpy(prop, uniqId, uEchoNodeProfileClassIdentificationUniqueIdLen);
  
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassOperatingStatus, propData, uEchoNodeProfileClassIdentificationNumberLen);
}

/****************************************
 * uecho_nodeprofileclass_setdefaultid
 ****************************************/

bool uecho_nodeprofileclass_setdefaultid(uEchoObject *obj)
{
  byte manCode[uEchoNodeProfileClassIdentificationManufacturerCodeLen];
  byte uniqId[uEchoNodeProfileClassIdentificationUniqueIdLen];
  
  memset(manCode, 0, sizeof(manCode));
  memset(uniqId, 0, sizeof(uniqId));
  
  return uecho_nodeprofileclass_setid(obj, manCode, uniqId);
}

/****************************************
 * uecho_nodeprofileclass_setinstancecount
 ****************************************/

bool uecho_nodeprofileclass_setinstancecount(uEchoObject *obj, int count)
{
  byte cntByte[uEchoNodeProfileClassNumberOfSelfNodeInstancesLen];
  
  uecho_integer2byte(count, cntByte, uEchoNodeProfileClassNumberOfSelfNodeInstancesLen);
  return uecho_object_setpropertydata(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances, cntByte, uEchoNodeProfileClassNumberOfSelfNodeInstancesLen);
}

/****************************************
 * uecho_nodeprofileclass_setclasscount
 ****************************************/

bool uecho_nodeprofileclass_setclasscount(uEchoObject *obj, int count)
{
  return true;
}

/****************************************
 * uecho_nodeprofileclass_updateinstanceproperties
 ****************************************/

bool uecho_nodeprofileclass_updateinstanceproperties(uEchoObject *obj)
{
  uEchoNode *node;
  uEchoObject *nodeObj;
  size_t nodeInstanceCnt;
  
  node = uecho_object_getparentnode(obj);
  if (!node)
    return false;
  
  nodeInstanceCnt = 0;
  for (nodeObj = uecho_node_getobjects(node); nodeObj; nodeObj = uecho_object_next(obj)) {
    nodeInstanceCnt++;
  }

  return true;
}

