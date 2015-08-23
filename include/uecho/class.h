/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CLASS_H_
#define _UECHO_CLASS_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>

#ifdef  __cplusplus
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
  
typedef struct _uEchoClass {
  bool headFlag;
  struct _uEchoClass *prev;
  struct _uEchoClass *next;

  byte code[2];
} uEchoClass, uEchoClassList;

/****************************************
 * Function
 ****************************************/

uEchoClass *uecho_class_new(void);
void uecho_class_delete(uEchoClass *cls);
#define uecho_class_next(cls) (uEchoClass *)uecho_list_next((uEchoList *)cls)
#define uecho_class_remove(cls) uecho_list_remove((uEchoList *)cls)

void uecho_class_setcode(uEchoClass *cls, uEchoClassCode code);
uEchoClassCode uecho_class_getcode(uEchoClass *cls);
bool uecho_class_iscode(uEchoClass *cls, uEchoClassCode code);
  
void uecho_class_setclassgroupcode(uEchoClass *cls, byte val);
byte uecho_class_getclassgroupcode(uEchoClass *cls);

void uecho_class_setclasscode(uEchoClass *cls, byte val);
byte uecho_class_getclasscode(uEchoClass *cls);

bool uecho_class_isdevice(uEchoClass *cls);
bool uecho_class_isprofile(uEchoClass *cls);

/****************************************
 * Function (Class List)
 ****************************************/
  
uEchoClassList *uecho_classlist_new(void);
void uecho_classlist_delete(uEchoClassList *clss);
  
bool uecho_classlist_set(uEchoClassList *clss, uEchoClassCode code);
uEchoClass *uecho_classlist_getbycode(uEchoClassList *clss, uEchoClassCode code);
  
#define uecho_classlist_clear(clss) uecho_list_clear((uEchoList *)clss, (UECHO_LIST_DESTRUCTORFUNC)uecho_class_delete)
#define uecho_classlist_size(clss) uecho_list_size((uEchoList *)clss)
#define uecho_classlist_gets(clss) (uEchoClass *)uecho_list_next((uEchoList *)clss)
#define uecho_classlist_add(clss,cls) uecho_list_add((uEchoList *)clss, (uEchoList *)cls)

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_CLASS_H_ */
