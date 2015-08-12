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
  uEchoProfileObjectSuperClassFaultEncountered   = 0x41,
  uEchoProfileObjectSuperClassNoFaultEncountered = 0x42
};
  
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

