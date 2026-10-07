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

#include <uecho/_manufacture.h>
#include <uecho/_object.h>
#include <uecho/_property.h>
#include <uecho/std/_standard.h>
#include <uecho/util/mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoDatabase {
  uEchoManufactureList* mans;
  uEchoObjectList* objs;
  // The standard database materializes entries from the const tables on demand.
  bool isStandard;
  uEchoMutex* mutex;
} uEchoDatabase;

/****************************************
 * Header
 ****************************************/

#include <uecho/std/database.h>

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_STD_DATABASE_INTERNAL_H_ */
