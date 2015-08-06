/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_UTIL_THREAD_H_
#define _UECHO_UTIL_THREAD_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>

#include <uecho/util/time.h>
#if defined(WIN32) && !defined(ITRON)
#include <windows.h>
#elif defined(BTRON)
#include <btron/proctask.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <btron/proctask.h>
#else
#include <pthread.h>
#include <signal.h>
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Define
 ****************************************/

#if defined (WINCE)
#define CG_THREAD_SHUTDOWN_ATTEMPTS 10
#define CG_THREAD_MIN_SLEEP 1000
/* ADD Fabrice Fontaine Orange 24/04/2007 */
/* Bug correction : Variable used to wait for thread termination by sleeping */
/* instead of joining */
#else
#define CG_THREAD_MIN_SLEEP 1000
/* ADD END Fabrice Fontaine Orange 24/04/2007 */ 
#endif

/****************************************
 * Data Type
 ****************************************/

/**
 * \brief The generic wrapper struct for CyberLinkC's threads.
 *
 * This wrapper has been created to enable 100% code
 * compatibility between different platforms (Linux, Win32 etc..)
 */
typedef struct _uEchoThread {
	bool headFlag;
	struct _uEchoThread *prev;
	struct _uEchoThread *next;
		
	/** Indicates whether this thread is ready to run */
	bool runnableFlag;
#if defined WINCE
	/** serves as look ahead to have the thread manage its own delete(thread) on exit */
	bool isRunning;
	bool deletePending;
	clock_t sleep;
#endif //WINCE

#if defined DEBUG
	char friendlyName[32];
#endif

#if defined(WIN32) && !defined(ITRON)
	HANDLE hThread;
	DWORD threadID;
#elif defined(BTRON)
	W taskID;
#elif defined(ITRON)
	ER_ID taskID;
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	ID taskID;
#elif defined(TENGINE) && defined(PROCESS_BASE)
 	WERR taskID;
#else

	/** The POSIX thread handle */
	pthread_t pThread;

#endif

	/** Thread's worker function */
	void (*action)(struct _uEchoThread *);

	/** Arbitrary data pointer */
	void *userData;
} uEchoThread, uEchoThreadList;

/**
 * Prototype for the threads' worker functions 
 */
typedef void (*uEchoThreadFunc)(uEchoThread *);

/****************************************
* Function
****************************************/

/**
 * Create a new thread
 */
uEchoThread *uecho_thread_new();

/**
 * Get a self reference to thread.
 */

uEchoThread *uecho_thread_self();

/**
 * Stop and destroy a thread.
 *
 * \param thread Thread to destroy
 */
bool uecho_thread_delete(uEchoThread *thread);

/**
 * Start a thread (must be created first with ch_thread_new())
 *
 * \param thread Thread to start
 */
bool uecho_thread_start(uEchoThread *thread);

/**
 * Stop a running thread.
 *
 * \param thread Thread to stop
 */
bool uecho_thread_stop(uEchoThread *thread);

/**
 * Restart a thread. Essentially calls uecho_thread_stop() and uecho_thread_start()
 *
 * \param thread Thread to restart
 */
bool uecho_thread_restart(uEchoThread *thread);

/**
 * Check if a thread has been started
 *
 * \param thread Thread to check
 */
bool uecho_thread_isrunnable(uEchoThread *thread);

/**
 * Set the thread's worker function.
 *
 * \param thread Thread struct
 * \param actionFunc Function pointer to set as the worker function
 */
void uecho_thread_setaction(uEchoThread *thread, uEchoThreadFunc actionFunc);

/**
 * Set the user data pointer
 *
 * \param thread Thread struct
 * \param data Pointer to user data
 */
void uecho_thread_setuserdata(uEchoThread *thread, void *data);

/**
 * Get the user data pointer
 *
 * \param thread Thread from which to get the pointer
 */
void *uecho_thread_getuserdata(uEchoThread *thread);

#if defined (WINCE)
void uecho_thread_sleep(uEchoThread *thread); 
void uecho_thread_exit(DWORD exitCode);
#if defined DEBUG_MEM
void uecho_thread_monitor(uEchoThread *thread);
#endif //DEBUG_MEM
#endif //WIN32_WCE

#define uecho_thread_next(thread) (uEchoThread *)uecho_list_next((uEchoList *)thread)
#define uecho_thread_remove(thread) uecho_list_remove((uEchoList *)thread)

/****************************************
* Function (Thread List)
****************************************/

/**
 * Create a new thread list
 *
 * \return Thread list
 */
uEchoThreadList *uecho_threadlist_new();

/**
 * Destroy a thread list
 *
 * \param threadList The thread list in question
 */
void uecho_threadlist_delete(uEchoThreadList *threadList);

/**
 * Clear the contents of a thread list
 *
 * \param threadList Thread list in question
 */
#define uecho_threadlist_clear(threadList) uecho_list_clear((uEchoList *)threadList, (CG_LIST_DESTRUCTORFUNC)uecho_thread_delete)

/**
 * Get the size of a thread list
 *
 * \param threadList The thread list in question
 */
#define uecho_threadlist_size(threadList) uecho_list_size((uEchoList *)threadList)

/**
 * Get the first actual item from a thread list to use as an iterator
 *
 * \param threadList The thread list in question
 */
#define uecho_threadlist_gets(threadList) (uEchoThread *)uecho_list_next((uEchoList *)threadList)

/**
 * Add a thread into a thread list
 *
 * \param threadList The thread list in question
 * \param thread The thread to add to the list
 */
#define uecho_threadlist_add(threadList, thread) uecho_list_add((uEchoList *)threadList, (uEchoList *)thread)

/**
 * Remove a thread from thread list
 *
 * \param threadList The thread list in question
 * \param thread The thread to be removed 
 */
#define uecho_threadlist_remove(thread) uecho_list_remove((uEchoList *)thread)

/**

 * Start all threads in the thread list
 *
 * \param threadList The thread list in question
 */
bool uecho_threadlist_start(uEchoThreadList *threadList);

/**
 * Stop all threads in the thread list
 *
 * \param threadList The thread list in question
 */
bool uecho_threadlist_stop(uEchoThreadList *threadList);

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
