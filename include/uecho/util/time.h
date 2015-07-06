/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_TIME_H_
#define _UECHO_UTIL_TIME_H_

#include <uecho/typedef.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Function
****************************************/

float uecho_random();

void uecho_wait(size_t mtime);
void uecho_waitrandom(size_t mtime);

#define uecho_sleep(val) uecho_wait(val)
#define uecho_sleeprandom(val) uecho_waitrandom(val)

size_t uecho_getcurrentsystemtime();

#ifdef  __cplusplus
}
#endif

#endif
