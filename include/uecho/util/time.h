/******************************************************************
 *
 * mUPnP for C
 *
 * Copyright (C) The uecho Authors 2005
 * Copyright (C) 2006 Nokia Corporation. All rights reserved.
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_TIME_H_
#define _UECHO_UTIL_TIME_H_

#include <uecho/typedef.h>

#if defined(BTRON)
#include <btron/proctask.h>
#include <btron/clk.h>
#elif defined(ITRON)
#include <kernel.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Define
****************************************/

#if defined(BTRON) || defined(ITRON) 
typedef size_t uEchoTime;
#else
typedef time_t uEchoTime;
#endif

/****************************************
* Function
****************************************/

float uecho_random();

void uecho_wait(uEchoTime mtime);
void uecho_waitrandom(uEchoTime mtime);

#define uecho_sleep(val) uecho_wait(val)
#define uecho_sleeprandom(val) uecho_waitrandom(val)

uEchoTime uecho_getcurrentsystemtime();

#ifdef  __cplusplus
}
#endif

#endif
