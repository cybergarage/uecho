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
 * Profile Object Super Class
 ****************************************/
  
enum {
  uEchoProfileObjectSuperClassFaultStatus                         = 0x88,
  uEchoProfileObjectSuperClassManufacturerCode                    = 0x8A,
  uEchoProfileObjectSuperClassPlaceOfBusinessCode                 = 0x8B,
  uEchoProfileObjectSuperClassProductCode                         = 0x8C,
  uEchoProfileObjectSuperClassSerialNumber                        = 0x8D,
  uEchoProfileObjectSuperClassDateOfManufacture                   = 0x8E,
  uEchoProfileObjectSuperClassStatusChangeAnnouncementPropertyMap = 0x9D,
  uEchoProfileObjectSuperClassSetPropertyMap                      = 0x9E,
  uEchoProfileObjectSuperClassGetPropertyMap                      = 0x9F,
} uEchoProfileObjectSuperClassEPC;

enum {
  uEchoProfileObjectSuperClassFaultStatusLen                            = 1,
  uEchoProfileObjectSuperClassManufacturerCodeLen                       = 3,
  uEchoProfileObjectSuperClassPlaceOfBusinessCodeLen                    = 3,
  uEchoProfileObjectSuperClassProductCodeLen                            = 12,
  uEchoProfileObjectSuperClassSerialNumberLen                           = 12,
  uEchoProfileObjectSuperClassDateOfManufactureLen                      = 4,
  uEchoProfileObjectSuperClassStatusChangeAnnouncementPropertyMapMaxLen = 17,
  uEchoProfileObjectSuperClassSetPropertyMapMaxLen                      = 17,
  uEchoProfileObjectSuperClassGetPropertyMapMaxLen                      = 17,
} uEchoProfileObjectSuperClassEPCSize;
  
enum {
  uEchoProfileObjectSuperClassFaultEncountered   = 0x41,
  uEchoProfileObjectSuperClassNoFaultEncountered = 0x42
};

bool uecho_object_addsuperclassproperties(uEchoObject *obj);
bool uecho_object_updatesuperclassproperties(uEchoObject *obj);
  
/****************************************
 * Node Profile Class
 ****************************************/

enum {
  uEchoProfileNodeProfileClassOperatingStatus           = 0x80,
  uEchoProfileNodeProfileClassVersionInformation        = 0x82,
  uEchoProfileNodeProfileClassIdentificationNumber      = 0x83,
  uEchoProfileNodeProfileClassFaultContent              = 0x89,
  uEchoProfileNodeProfileClassUniqueIdentifierData      = 0xBF,
  uEchoProfileNodeProfileClassNumberOfSelfNodeInstances = 0xD3,
  uEchoProfileNodeProfileClassNumberOfSelfNodeClasses   = 0xD4,
  uEchoProfileNodeProfileClassInstanceListNotification  = 0xD5,
  uEchoProfileNodeProfileClassSelfNodeInstanceListS     = 0xD6,
  uEchoProfileNodeProfileClassSelfNodeClassListS        = 0xD7,
};
  
enum {
  uEchoProfileNodeProfileClassBooting    = 0x30,
  uEchoProfileNodeProfileClassNotBooting = 0x31,
};

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
