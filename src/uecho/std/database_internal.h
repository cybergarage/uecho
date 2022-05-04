/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_STD_DATABASE_INTERNAL_H_
#define _UECHO_STD_DATABASE_INTERNAL_H_

#include <uecho/typedef.h>

#include <uecho/manufacture_internal.h>
#include <uecho/object_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoDatabase {
  uEchoManufactureList *mans;
  uEchoObjectList *objs;
} uEchoDatabase;

/****************************************
 * Header
 ****************************************/

#include <uecho/std/database.h>

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_STD_DATABASE_INTERNAL_H_ */
