/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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

float uecho_random(void);

void uecho_wait(clock_t mtime);
void uecho_waitrandom(clock_t mtime);

#define uecho_sleep(val) uecho_wait(val)
#define uecho_sleeprandom(val) uecho_waitrandom(val)

clock_t uecho_getcurrentsystemtime(void);

#ifdef  __cplusplus
}
#endif

#endif
