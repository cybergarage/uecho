/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MANUFACTURE_H_
#define _UECHO_MANUFACTURE_H_

#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef int uEchoManufactureCode;

#if !defined(_UECHO_MANUFACTURE_INTERNAL_H_)
typedef void uEchoManufacture;
#endif

/****************************************
 * Function (Manufacture)
 ****************************************/

uEchoManufacture* uecho_manufacture_new(void);
bool uecho_manufacture_delete(uEchoManufacture* man);
uEchoManufacture* uecho_manufacture_next(uEchoManufacture* man);

void uecho_manufacture_setcode(uEchoManufacture* man, uEchoManufactureCode code);
uEchoManufactureCode uecho_manufacture_getcode(uEchoManufacture* man);
bool uecho_manufacture_iscode(uEchoManufacture* man, uEchoManufactureCode code);

void uecho_manufacture_setname(uEchoManufacture* man, const char *name);
const char *uecho_manufacture_getname(uEchoManufacture* man);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MANUFACTURE_H_ */
