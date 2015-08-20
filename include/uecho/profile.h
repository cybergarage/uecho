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
  uEchoProfileSuperClassFaultStatus                         = 0x88,
  uEchoProfileSuperClassManufacturerCode                    = 0x8A,
  uEchoProfileSuperClassPlaceOfBusinessCode                 = 0x8B,
  uEchoProfileSuperClassProductCode                         = 0x8C,
  uEchoProfileSuperClassSerialNumber                        = 0x8D,
  uEchoProfileSuperClassDateOfManufacture                   = 0x8E,
  uEchoProfileSuperClassAnnoPropertyMap                     = 0x9D,
  uEchoProfileSuperClassSetPropertyMap                      = 0x9E,
  uEchoProfileSuperClassGetPropertyMap                      = 0x9F,
} uEchoProfileSuperClassEPC;

enum {
  uEchoPropertyMapMaxLen = 16,
};

typedef enum {
  uEchoProfileSuperClassFaultStatusLen                            = 1,
  uEchoProfileSuperClassManufacturerCodeLen                       = 3,
  uEchoProfileSuperClassPlaceOfBusinessCodeLen                    = 3,
  uEchoProfileSuperClassProductCodeLen                            = 12,
  uEchoProfileSuperClassSerialNumberLen                           = 12,
  uEchoProfileSuperClassDateOfManufactureLen                      = 4,
  uEchoProfileSuperClassAnnoPropertyMapMaxLen = (uEchoPropertyMapMaxLen + 1),
  uEchoProfileSuperClassSetPropertyMapMaxLen                      = (uEchoPropertyMapMaxLen + 1),
  uEchoProfileSuperClassGetPropertyMapMaxLen                      = (uEchoPropertyMapMaxLen + 1),
} uEchoProfileSuperClassEPCSize;
  
enum {
  uEchoProfileSuperClassFaultEncountered   = 0x41,
  uEchoProfileSuperClassNoFaultEncountered = 0x42
};

bool uecho_profile_superclass_addmandatoryproperties(uEchoObject *obj);
bool uecho_profile_superclass_setmanufacturercode(uEchoObject *obj, byte *codes);
bool uecho_profile_superclass_updatepropertymaps(uEchoObject *obj);
void uecho_profile_superclass_clearpropertymapcaches(uEchoObject *obj);
  
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

bool uecho_nodeprofileclass_addmandatoryproperties(uEchoObject *obj);
bool uecho_nodeprofileclass_setoperatingstatus(uEchoObject *obj, bool stats);
bool uecho_nodeprofileclass_setid(uEchoObject *obj, byte *manCode, byte *uniqId);

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
