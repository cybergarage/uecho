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

/****************************************
 * Data Type
 ****************************************/

typedef unsigned int uEchoClassCode;
  
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
void uecho_class_delete(uEchoClass *obj);
#define uecho_class_next(obj) (uEchoClass *)uecho_list_next((uEchoList *)obj)
#define uecho_class_remove(obj) uecho_list_remove((uEchoList *)obj)

void uecho_class_setcode(uEchoClass *obj, uEchoClassCode code);
uEchoClassCode uecho_class_getcode(uEchoClass *obj);
bool uecho_class_iscode(uEchoClass *obj, uEchoClassCode code);
  
void uecho_class_setclassgroupcode(uEchoClass *obj, byte val);
byte uecho_class_getclassgroupcode(uEchoClass *obj);

void uecho_class_setclasscode(uEchoClass *obj, byte val);
byte uecho_class_getclasscode(uEchoClass *obj);

/****************************************
 * Function (Class List)
 ****************************************/
  
uEchoClassList *uecho_classlist_new(void);
void uecho_classlist_delete(uEchoClassList *objs);
  
bool uecho_classlist_set(uEchoClassList *props, uEchoClassCode code);
uEchoClass *uecho_classlist_getbycode(uEchoClassList *props, uEchoClassCode code);
  
#define uecho_classlist_clear(objs) uecho_list_clear((uEchoList *)objs, (UECHO_LIST_DESTRUCTORFUNC)uecho_class_delete)
#define uecho_classlist_size(objs) uecho_list_size((uEchoList *)objs)
#define uecho_classlist_gets(objs) (uEchoClass *)uecho_list_next((uEchoList *)objs)
#define uecho_classlist_add(objs,obj) uecho_list_add((uEchoList *)objs, (uEchoList *)obj)

#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_CLASS_H_ */
