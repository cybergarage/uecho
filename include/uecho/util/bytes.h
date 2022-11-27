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

int uecho_bytes_toint(byte* byte_data, size_t byte_size);
byte* uecho_bytes_fromint(int val, size_t bytes_size);

#ifdef __cplusplus
}
#endif

#endif
