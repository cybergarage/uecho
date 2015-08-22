/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROFILE_H_
#define _UECHO_PROFILE_H_

#include <uecho/object.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Profile
 ****************************************/

uEchoObject *uecho_profile_new(void);
  
/****************************************
 * Profile Object Super Class
 ****************************************/
  
typedef enum {
  uEchoProfileFaultStatus                         = 0x88,
  uEchoProfileManufacturerCode                    = uEchoObjectManufacturerCode,
  uEchoProfilePlaceOfBusinessCode                 = 0x8B,
  uEchoProfileProductCode                         = 0x8C,
  uEchoProfileSerialNumber                        = 0x8D,
  uEchoProfileDateOfManufacture                   = 0x8E,
  uEchoProfileAnnoPropertyMap                     = uEchoObjectAnnoPropertyMap,
  uEchoProfileSetPropertyMap                      = uEchoObjectSetPropertyMap,
  uEchoProfileGetPropertyMap                      = uEchoObjectGetPropertyMap,
} uEchoProfileEPC;

enum {
  uEchoPropertyMapMaxLen = 16,
};

typedef enum {
  uEchoProfileFaultStatusLen          = 1,
  uEchoProfileManufacturerCodeLen     = uEchoObjectManufacturerCodeLen,
  uEchoProfilePlaceOfBusinessCodeLen  = 3,
  uEchoProfileProductCodeLen          = 12,
  uEchoProfileSerialNumberLen         = 12,
  uEchoProfileDateOfManufactureLen    = 4,
  uEchoProfileAnnoPropertyMapMaxLen   = uEchoObjectAnnoPropertyMapMaxLen,
  uEchoProfileSetPropertyMapMaxLen    = uEchoObjectSetPropertyMap,
  uEchoProfileGetPropertyMapMaxLen    = uEchoObjectGetPropertyMap,
} uEchoProfileEPCSize;
  
enum {
  uEchoProfileFaultEncountered   = 0x41,
  uEchoProfileNoFaultEncountered = 0x42
};

bool uecho_profile_addmandatoryproperties(uEchoObject *obj);
bool uecho_profile_setmanufacturercode(uEchoObject *obj, byte *codes);
  
/****************************************
 * Node Profile Class
 ****************************************/

enum {
  uEchoNodeProfileClassOperatingStatus           = 0x80,
  uEchoNodeProfileClassVersionInformation        = 0x82,
  uEchoNodeProfileClassIdentificationNumber      = 0x83,
  uEchoNodeProfileClassFaultContent              = 0x89,
  uEchoNodeProfileClassUniqueIdentifierData      = 0xBF,
  uEchoNodeProfileClassNumberOfSelfNodeInstances = 0xD3,
  uEchoNodeProfileClassNumberOfSelfNodeClasses   = 0xD4,
  uEchoNodeProfileClassInstanceListNotification  = 0xD5,
  uEchoNodeProfileClassSelfNodeInstanceListS     = 0xD6,
  uEchoNodeProfileClassSelfNodeClassListS        = 0xD7,
};

enum {
  uEchoNodeProfileClassOperatingStatusLen                 = 1,
  uEchoNodeProfileClassVersionInformationLen              = 4,
  uEchoNodeProfileClassIdentificationManufacturerCodeLen  = 3,
  uEchoNodeProfileClassIdentificationUniqueIdLen          = 13,
  uEchoNodeProfileClassIdentificationNumberLen            = 1 + uEchoNodeProfileClassIdentificationManufacturerCodeLen + uEchoNodeProfileClassIdentificationUniqueIdLen,
  uEchoNodeProfileClassFaultContentLen                    = 2,
  uEchoNodeProfileClassUniqueIdentifierDataLen            = 2,
  uEchoNodeProfileClassNumberOfSelfNodeInstancesLen       = 3,
  uEchoNodeProfileClassNumberOfSelfNodeClassesLen         = 2,
  uEchoNodeProfileClassInstanceListNotificationMaxLen     = 253,
  uEchoNodeProfileClassSelfNodeInstanceListSMaxLen        = 253,
  uEchoNodeProfileClassSelfNodeClassListSMaxLen           = 17,
};

enum {
  uEchoNodeProfileClassBooting      = 0x30,
  uEchoNodeProfileClassNotBooting   = 0x31,
  uEchoLowerCommunicationLayerProtocolType = 0xFE,
};

uEchoObject *uecho_nodeprofileclass_new(void);
bool uecho_nodeprofileclass_addmandatoryproperties(uEchoObject *obj);

bool uecho_nodeprofileclass_setoperatingstatus(uEchoObject *obj, bool stats);
bool uecho_nodeprofileclass_setid(uEchoObject *obj, byte *manCode, byte *uniqId);
bool uecho_nodeprofileclass_updateinstanceproperties(uEchoObject *obj);

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
