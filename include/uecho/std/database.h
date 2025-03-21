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

#include <uecho/manufacture.h>
#include <uecho/object.h>
#include <uecho/typedef.h>

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

bool uecho_database_addmanufacture(uEchoDatabase* db, uEchoManufacture* man);
bool uecho_database_addobject(uEchoDatabase* db, uEchoObject* obj);

uEchoManufacture* uecho_database_getmanufacture(uEchoDatabase* db, uEchoManufactureCode code);
uEchoObject* uecho_database_getobject(uEchoDatabase* db, byte grpCode, byte clsCode);

uEchoDatabase* uecho_standard_getdatabase(void);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_STD_DATABASE_H_ */
