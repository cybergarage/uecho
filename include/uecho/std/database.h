/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_STD_DATABASE_H_
#define _UECHO_STD_DATABASE_H_

#include <uecho/typedef.h>
#include <uecho/manufacture.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

#if !defined(_UECHO_STD_DATABASE_INTERNAL_H_)
typedef void uEchoDatabase;
#endif

/****************************************
 * Function (Database)
 ****************************************/

uEchoDatabase* uecho_database_new(void);
bool uecho_database_delete(uEchoDatabase* db);

bool uecho_database_addmanufacture(uEchoDatabase* db, uEchoManufacture *man);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_STD_DATABASE_H_ */
