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

#include <uecho/util/timer.h>
#if defined(WIN32)
#include <windows.h>
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

#define UECHO_THREAD_MIN_SLEEP 500

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoThread {
  UECHO_LIST_STRUCT_MEMBERS
    
  bool runnableFlag;

#if defined(WIN32)
  HANDLE hThread;
  DWORD threadID;
#else
  pthread_t pThread;
#endif

  void (*action)(struct _uEchoThread *);
  void *userData;
} uEchoThread, uEchoThreadList;

typedef void (*uEchoThreadFunc)(uEchoThread *);

/****************************************
* Function
****************************************/

uEchoThread *uecho_thread_new(void);
bool uecho_thread_delete(uEchoThread *thread);

bool uecho_thread_start(uEchoThread *thread);
bool uecho_thread_stop(uEchoThread *thread);
bool uecho_thread_restart(uEchoThread *thread);
bool uecho_thread_isrunnable(uEchoThread *thread);
bool uecho_thread_isrunning(uEchoThread *thread);
  
void uecho_thread_setaction(uEchoThread *thread, uEchoThreadFunc actionFunc);
void uecho_thread_setuserdata(uEchoThread *thread, void *data);
void *uecho_thread_getuserdata(uEchoThread *thread);

#define uecho_thread_next(thread) (uEchoThread *)uecho_list_next((uEchoList *)thread)
#define uecho_thread_remove(thread) uecho_list_remove((uEchoList *)thread)

/****************************************
* Function (Thread List)
****************************************/

uEchoThreadList *uecho_threadlist_new();
void uecho_threadlist_delete(uEchoThreadList *threadList);

#define uecho_threadlist_clear(threadList) uecho_list_clear((uEchoList *)threadList, (UECHO_LIST_DESTRUCTORFUNC)uecho_thread_delete)
#define uecho_threadlist_size(threadList) uecho_list_size((uEchoList *)threadList)
#define uecho_threadlist_gets(threadList) (uEchoThread *)uecho_list_next((uEchoList *)threadList)
#define uecho_threadlist_add(threadList, thread) uecho_list_add((uEchoList *)threadList, (uEchoList *)thread)
#define uecho_threadlist_remove(thread) uecho_list_remove((uEchoList *)thread)

bool uecho_threadlist_start(uEchoThreadList *threadList);
bool uecho_threadlist_stop(uEchoThreadList *threadList);

#ifdef  __cplusplus

} /* extern "C" */

#endif

#endif
