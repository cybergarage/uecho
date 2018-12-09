/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CLASS_H_
#define _UECHO_CLASS_H_

#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoClassCodeMin = 0x0000,
  uEchoClassCodeMax = 0xFFFF,
};

enum {
  uEchoClassGroupDeviceMin = 0x00,
  uEchoClassGroupDeviceMax = 0x06,
  uEchoClassGroupProfile = 0x0E,
};

/****************************************
 * Data Type
 ****************************************/

typedef int uEchoClassCode;

#if !defined(_UECHO_CLASS_INTERNAL_H_)
typedef void uEchoClass;
#endif

/****************************************
 * Function
 ****************************************/

uEchoClass* uecho_class_new(void);
bool uecho_class_delete(uEchoClass* cls);
uEchoClass* uecho_class_next(uEchoClass* cls);

void uecho_class_setcode(uEchoClass* cls, uEchoClassCode code);
uEchoClassCode uecho_class_getcode(uEchoClass* cls);
bool uecho_class_iscode(uEchoClass* cls, uEchoClassCode code);

void uecho_class_setclassgroupcode(uEchoClass* cls, byte val);
byte uecho_class_getclassgroupcode(uEchoClass* cls);

void uecho_class_setclasscode(uEchoClass* cls, byte val);
byte uecho_class_getclasscode(uEchoClass* cls);

bool uecho_class_isdevice(uEchoClass* cls);
bool uecho_class_isprofile(uEchoClass* cls);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_CLASS_H_ */
