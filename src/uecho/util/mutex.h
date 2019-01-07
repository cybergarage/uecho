/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_MUTEX_H_
#define _UECHO_UTIL_MUTEX_H_

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

typedef struct _uEchoMutex {
#if defined(WIN32)
  HANDLE mutexID;
#else
  pthread_mutex_t mutexID;
#endif
} uEchoMutex;

/****************************************
 * Functions
 ****************************************/

uEchoMutex* uecho_mutex_new(void);
bool uecho_mutex_delete(uEchoMutex* mutex);

bool uecho_mutex_lock(uEchoMutex* mutex);
bool uecho_mutex_unlock(uEchoMutex* mutex);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
