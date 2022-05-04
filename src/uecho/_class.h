/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CLASS_INTERNAL_H_
#define _UECHO_CLASS_INTERNAL_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct {
  UECHO_LIST_STRUCT_MEMBERS

  byte code[2];
} uEchoClass, uEchoClassList;

/****************************************
 * Header
 ****************************************/

#include <uecho/class.h>

/****************************************
 * Function
 ****************************************/

#define uecho_class_remove(cls) uecho_list_remove((uEchoList*)cls)

/****************************************
 * Function (Class List)
 ****************************************/

uEchoClassList* uecho_classlist_new(void);
void uecho_classlist_delete(uEchoClassList* clss);

bool uecho_classlist_set(uEchoClassList* clss, uEchoClassCode code);
uEchoClass* uecho_classlist_getbycode(uEchoClassList* clss, uEchoClassCode code);

#define uecho_classlist_clear(clss) uecho_list_clear((uEchoList*)clss, (UECHO_LIST_DESTRUCTORFUNC)uecho_class_delete)
#define uecho_classlist_size(clss) uecho_list_size((uEchoList*)clss)
#define uecho_classlist_gets(clss) (uEchoClass*)uecho_list_next((uEchoList*)clss)
#define uecho_classlist_add(clss, cls) uecho_list_add((uEchoList*)clss, (uEchoList*)cls)

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_CLASS_H_ */
