/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MANUFACTURE_INTERNAL_H_
#define _UECHO_MANUFACTURE_INTERNAL_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/strings.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoManufacture {
  UECHO_LIST_STRUCT_MEMBERS

  int code;
  uEchoString *name;
} uEchoManufacture, uEchoManufactureList;

/****************************************
 * Header
 ****************************************/

#include <uecho/manufacture.h>

/****************************************
 * Function (Object List)
 ****************************************/

uEchoManufactureList* uecho_manufacturelist_new(void);
void uecho_manufacturelist_delete(uEchoManufactureList* mans);

uEchoManufacture* uecho_manufacturelist_getbycode(uEchoManufactureList* mans, uEchoManufactureCode code);

#define uecho_manufacturelist_clear(mans) uecho_list_clear((uEchoList*)mans, (UECHO_LIST_DESTRUCTORFUNC)uecho_manufacture_delete)
#define uecho_manufacturelist_size(mans) uecho_list_size((uEchoList*)mans)
#define uecho_manufacturelist_gets(mans) (uEchoManufacture*)uecho_list_next((uEchoList*)mans)
#define uecho_manufacturelist_add(mans, obj) uecho_list_add((uEchoList*)mans, (uEchoList*)obj)

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MANUFACTURE_INTERNAL_H_ */
