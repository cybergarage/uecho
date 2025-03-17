/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_BYTES_H_
#define _UECHO_UTIL_BYTES_H_

#include <uecho/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Function
 ****************************************/

int uecho_bytes_toint(byte* byteData, size_t byteSize);
byte* uecho_bytes_fromint(int val, size_t bytesSize);

#ifdef __cplusplus
}
#endif

#endif
