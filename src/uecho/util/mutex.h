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
#elif defined(__ESP32__)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#else
#include <pthread.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

typedef struct UEchoMutex {
#if defined(WIN32)
  HANDLE mutexId;
#elif defined(__ESP32__)
  SemaphoreHandle_t mutex;
#else
  pthread_mutex_t mutexId;
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
