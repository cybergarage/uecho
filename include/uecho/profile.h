/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROFILE_H_
#define _UECHO_PROFILE_H_

#include <uecho/const.h>
#include <uecho/object.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Profile
 ****************************************/

uEchoObject* uecho_profile_new(void);

/****************************************
 * Profile Object Super Class
 ****************************************/

typedef enum {
  uEchoProfileFaultStatus = 0x88,
  uEchoProfileManufacturerCode = uEchoObjectManufacturerCode,
  uEchoProfilePlaceOfBusinessCode = 0x8B,
  uEchoProfileProductCode = 0x8C,
  uEchoProfileSerialNumber = 0x8D,
  uEchoProfileDateOfManufacture = 0x8E,
  uEchoProfileAnnoPropertyMap = uEchoObjectAnnoPropertyMap,
  uEchoProfileSetPropertyMap = uEchoObjectSetPropertyMap,
  uEchoProfileGetPropertyMap = uEchoObjectGetPropertyMap,
} uEchoProfileEPC;

enum {
  uEchoPropertyMapFormat1MaxSize = 15,
  uEchoPropertyMapFormat2MapSize = 16,
  uEchoPropertyMapFormat2Size = (uEchoPropertyMapFormat2MapSize + 1),
  uEchoPropertyMapFormatMaxSize = uEchoPropertyMapFormat2Size,
};

typedef enum {
  uEchoProfileFaultStatusLen = 1,
  uEchoProfileManufacturerCodeLen = uEchoObjectManufacturerCodeLen,
  uEchoProfilePlaceOfBusinessCodeLen = 3,
  uEchoProfileProductCodeLen = 12,
  uEchoProfileSerialNumberLen = 12,
  uEchoProfileDateOfManufactureLen = 4,
  uEchoProfileAnnoPropertyMapMaxLen = uEchoObjectAnnoPropertyMapMaxLen,
  uEchoProfileSetPropertyMapMaxLen = uEchoObjectSetPropertyMap,
  uEchoProfileGetPropertyMapMaxLen = uEchoObjectGetPropertyMap,
} uEchoProfileEPCSize;

enum {
  uEchoProfileFaultEncountered = 0x41,
  uEchoProfileNoFaultEncountered = 0x42
};

bool uecho_profile_addmandatoryproperties(uEchoObject* obj);

bool uecho_profile_setmanufacturercode(uEchoObject* obj, uEchoObjectCode code);
bool uecho_profile_getmanufacturercode(uEchoObject* obj);

/****************************************
 * Node Profile Class
 ****************************************/

enum {
  uEchoNodeProfileClassOperatingStatus = 0x80,
  uEchoNodeProfileClassVersionInformation = 0x82,
  uEchoNodeProfileClassIdentificationNumber = 0x83,
  uEchoNodeProfileClassFaultContent = 0x89,
  uEchoNodeProfileClassUniqueIdentifierData = 0xBF,
  uEchoNodeProfileClassNumberOfSelfNodeInstances = 0xD3,
  uEchoNodeProfileClassNumberOfSelfNodeClasses = 0xD4,
  uEchoNodeProfileClassInstanceListNotification = 0xD5,
  uEchoNodeProfileClassSelfNodeInstanceListS = 0xD6,
  uEchoNodeProfileClassSelfNodeClassListS = 0xD7,
};

enum {
  uEchoNodeProfileClassOperatingStatusLen = 1,
  uEchoNodeProfileClassVersionInformationLen = 4,
  uEchoNodeProfileClassIdentificationManufacturerCodeLen = 3,
  uEchoNodeProfileClassIdentificationUniqueIdLen = 13,
  uEchoNodeProfileClassIdentificationNumberLen = 1 + uEchoNodeProfileClassIdentificationManufacturerCodeLen + uEchoNodeProfileClassIdentificationUniqueIdLen,
  uEchoNodeProfileClassFaultContentLen = 2,
  uEchoNodeProfileClassUniqueIdentifierDataLen = 2,
  uEchoNodeProfileClassNumberOfSelfNodeInstancesLen = 3,
  uEchoNodeProfileClassNumberOfSelfNodeClassesLen = 2,
  uEchoNodeProfileClassSelfNodeInstanceListSMax = 0xFF,
  uEchoNodeProfileClassSelfNodeClassListSMax = 0xFF,
  uEchoNodeProfileClassInstanceListNotificationMax = uEchoNodeProfileClassSelfNodeInstanceListSMax,
};

enum {
  uEchoNodeProfileClassBooting = 0x30,
  uEchoNodeProfileClassNotBooting = 0x31,
  uEchoLowerCommunicationLayerProtocolType = 0xFE,
};

uEchoObject* uecho_nodeprofile_new(void);
bool uecho_nodeprofile_addmandatoryproperties(uEchoObject* obj);

bool uecho_nodeprofile_setoperatingstatus(uEchoObject* obj, bool stats);
bool uecho_nodeprofile_setversion(uEchoObject* obj, int major_ver, int minor_ver);
bool uecho_nodeprofile_setid(uEchoObject* obj, byte* man_code, byte* uniq_id);
bool uecho_nodeprofile_setdefaultid(uEchoObject* obj);
bool uecho_nodeprofile_setinstancecount(uEchoObject* obj, int count);
bool uecho_nodeprofile_setclasscount(uEchoObject* obj, int count);
bool uecho_nodeprofile_setinstancelist(uEchoObject* obj, int list_cnt, byte* list_bytes);
bool uecho_nodeprofile_setclasslist(uEchoObject* obj, int list_cnt, byte* list_bytes);

bool uecho_nodeprofile_isoperatingstatus(uEchoObject* obj);
int uecho_nodeprofile_getinstancecount(uEchoObject* obj);
int uecho_nodeprofile_getclasscount(uEchoObject* obj);
byte* uecho_nodeprofile_getnotificationinstancelist(uEchoObject* obj);
byte* uecho_nodeprofile_getinstancelist(uEchoObject* obj);
byte* uecho_nodeprofile_getclasslist(uEchoObject* obj);

bool uecho_nodeprofile_updateinstanceproperties(uEchoObject* obj);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
