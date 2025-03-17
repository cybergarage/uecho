/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_COND_H_
#define _UECHO_UTIL_COND_H_

#include <uecho/typedef.h>

#if defined(WIN32)
#include <winsock2.h>
#else
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

typedef struct UEchoCond {
  pthread_mutex_t mutexId;
  pthread_cond_t condId;
} uEchoCond;

/****************************************
 * Functions
 ****************************************/

uEchoCond* uecho_cond_new(void);
bool uecho_cond_delete(uEchoCond* cond);

bool uecho_cond_wait(uEchoCond* cond);
bool uecho_cond_timedwait(uEchoCond* cond, clock_t mtime);
bool uecho_cond_signal(uEchoCond* cond);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
