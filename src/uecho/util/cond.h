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
#elif defined(__ESP32__)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#else
#include <pthread.h>
#endif

#include <uecho/util/mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

typedef struct UEchoCond {
#if defined(WIN32)
  HANDLE condId;
#elif defined(__ESP32__)
  SemaphoreHandle_t sem;
#else
  pthread_cond_t condId;
#endif
} uEchoCond;

/****************************************
 * Functions
 ****************************************/

uEchoCond* uecho_cond_new(void);
bool uecho_cond_delete(uEchoCond* cond);

#if defined(__ESP32__)
bool uecho_cond_wait(uEchoCond* cond, uEchoMutex* mutex, uint32_t timeout_ms);
#else
bool uecho_cond_wait(uEchoCond* cond);
bool uecho_cond_timedwait(uEchoCond* cond, clock_t mtime);
#endif
bool uecho_cond_signal(uEchoCond* cond);

#ifdef __cplusplus

} /* extern "C" */

#endif

#endif
