/******************************************************************
 *
 * mUPnP for C
 *
 * Copyright (C) Satoshi Konno 2005
 * Copyright (C) 2006 Nokia Corporation. All rights reserved.
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_MUTEX_H_
#define _UECHO_UTIL_MUTEX_H_

#include <uecho/typedef.h>

#if defined(WIN32) && !defined(ITRON)
#include <winsock2.h>
#elif defined(BTRON)
#include <btron/taskcomm.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/taskcomm.h>
#else
#include <pthread.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Types
 ****************************************/

/**
 * \brief The generic wrapper struct for CyberLinkC's mutexes.
 *
 * This wrapper has been created to enable 100% code
 * compatibility for different platforms (Linux, Win32 etc..)
 */
typedef struct _uEchoMutex {
#if defined(WIN32) && !defined(ITRON)
	HANDLE	mutexID;
#elif defined(BTRON)
	WERR	mutexID;
#elif defined(ITRON)
	ER_ID	mutexID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	ID mutexID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
	WERR	mutexID;
#else
	/** The mutex entity */
	pthread_mutex_t mutexID;
#endif
} uEchoMutex;

#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#include <stdlib.h>

typedef struct _uEchoLockInfo
{
	struct _uEchoLockInfo *next;
	pthread_t thread_id;
	char *file, *function;
	int line, mutex_id;
} uEchoLockInfo;
#endif

/****************************************
 * Functions
 ****************************************/

/** 
 * Create a new mutex
 */
uEchoMutex *uecho_mutex_new();

/** 
 * Destroy a mutex
 *
 * \param mutex The mutex to destroy
 */
BOOL uecho_mutex_delete(uEchoMutex *mutex);

/** 
 * Acquire a mutex lock
 *
 * \param mutex Mutex to lock
 */

#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#	define uecho_mutex_lock(mutex) uecho_mutex_lock_trace(__FILE__,  __LINE__, __PRETTY_FUNCTION__, mutex)
#else
	BOOL uecho_mutex_lock(uEchoMutex *mutex);
#endif

/** 
 * Release a locked mutex
 *
 * \param mutex Mutex to unlock
 */
#if defined(WITH_THREAD_LOCK_TRACE) && defined(__USE_ISOC99)
#       define uecho_mutex_unlock(mutex) uecho_mutex_unlock_trace(__FILE__,  __LINE__, __PRETTY_FUNCTION__, mutex)
#else
	BOOL uecho_mutex_unlock(uEchoMutex *mutex);
#endif

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
