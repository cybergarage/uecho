/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#if !defined(WIN32) && !defined(ESP_PLATFORM)
#include <signal.h>
#endif

#include <string.h>
#include <uecho/util/thread.h>
#include <uecho/util/timer.h>

#if !defined(WIN32) && !defined(ESP_PLATFORM)
static void uecho_sig_handler(int sign);
#endif

/****************************************
 * Thread Function
 ****************************************/

#if defined(WIN32)
static DWORD WINAPI Win32ThreadProc(LPVOID lpParam)
{
  uEchoThread* thread;

  thread = (uEchoThread*)lpParam;
  if (thread->action != NULL)
    thread->action(thread);

  return 0;
}
#elif defined(ESP_PLATFORM)
static void* posix_thread_proc(void* param)
{
  uEchoThread* thread = (uEchoThread*)param;

  /* ESP-IDF has no POSIX signals; workers stop cooperatively and are joined. */
  thread->task = xTaskGetCurrentTaskHandle();
  if (thread->action != NULL)
    thread->action(thread);

  return NULL;
}
#else
static void* posix_thread_proc(void* param)
{
  sigset_t set;
  struct sigaction actions;
  uEchoThread* thread = (uEchoThread*)param;

  sigfillset(&set);
  sigdelset(&set, SIGQUIT);
  pthread_sigmask(SIG_SETMASK, &set, NULL);

  memset(&actions, 0, sizeof(actions));
  sigemptyset(&actions.sa_mask);
  actions.sa_flags = 0;
  actions.sa_handler = uecho_sig_handler;
  sigaction(SIGQUIT, &actions, NULL);

  if (thread->action != NULL)
    thread->action(thread);

  return 0;
}
#endif

/****************************************
 * uecho_thread_new
 ****************************************/

uEchoThread* uecho_thread_new(void)
{
  uEchoThread* thread;

  thread = (uEchoThread*)malloc(sizeof(uEchoThread));

  if (!thread)
    return NULL;

  uecho_list_node_init((uEchoList*)thread);

  thread->runnableFlag = false;
  thread->action = NULL;
  thread->userData = NULL;
#if !defined(WIN32)
  thread->joinable = false;
#endif
#if defined(ESP_PLATFORM)
  thread->task = NULL;
#endif

  return thread;
}

/****************************************
 * uecho_thread_delete
 ****************************************/

bool uecho_thread_delete(uEchoThread* thread)
{
  if (!thread)
    return false;

#if !defined(WIN32)
  /* Also joins a worker whose action already returned without an explicit stop. */
  uecho_thread_stop(thread);
#else
  if (thread->runnableFlag == true) {
    uecho_thread_stop(thread);
  }
#endif

  uecho_thread_remove(thread);

  free(thread);

  return true;
}

/****************************************
 * uecho_thread_start
 ****************************************/

bool uecho_thread_start(uEchoThread* thread)
{
  if (!thread)
    return false;

  thread->runnableFlag = true;

#if defined(WIN32)
  thread->hThread = CreateThread(NULL, 0, Win32ThreadProc, (LPVOID)thread, 0, &thread->threadID);
#elif defined(ESP_PLATFORM)
  pthread_attr_t threadAttr;
  if (thread->joinable) {
    /* Reap a previous run before reusing this object. */
    uecho_thread_stop(thread);
    thread->runnableFlag = true;
  }

  if (pthread_attr_init(&threadAttr) != 0) {
    thread->runnableFlag = false;
    return false;
  }

  /* Keep workers joinable so stop() can wait before their objects are freed. */
  if ((pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE) != 0) || (pthread_attr_setstacksize(&threadAttr, UECHO_THREAD_STACK_SIZE) != 0)) {
    thread->runnableFlag = false;
    pthread_attr_destroy(&threadAttr);
    return false;
  }

  if (pthread_create(&thread->pThread, &threadAttr, posix_thread_proc, thread) != 0) {
    thread->runnableFlag = false;
    pthread_attr_destroy(&threadAttr);
    return false;
  }
  thread->joinable = true;
  pthread_attr_destroy(&threadAttr);
#else
  pthread_attr_t threadAttr;
  if (thread->joinable) {
    /* Reap a previous run before reusing this object. */
    uecho_thread_stop(thread);
    thread->runnableFlag = true;
  }

  if (pthread_attr_init(&threadAttr) != 0) {
    thread->runnableFlag = false;
    return false;
  }

  /* Keep workers joinable so stop() can wait before their objects are freed. */
  if (pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_JOINABLE) != 0) {
    thread->runnableFlag = false;
    pthread_attr_destroy(&threadAttr);
    return false;
  }

  if (pthread_create(&thread->pThread, &threadAttr, posix_thread_proc, thread) != 0) {
    thread->runnableFlag = false;
    pthread_attr_destroy(&threadAttr);
    return false;
  }
  thread->joinable = true;
  pthread_attr_destroy(&threadAttr);
#endif

  return true;
}

/****************************************
 * uecho_thread_stop
 ****************************************/

bool uecho_thread_stop(uEchoThread* thread)
{
  if (!thread)
    return false;

#if defined(ESP_PLATFORM)
  thread->runnableFlag = false;
  if (thread->joinable) {
    thread->joinable = false;
    if (thread->task == xTaskGetCurrentTaskHandle()) {
      /* A worker stopping itself cannot join; let it release itself on exit. */
      pthread_detach(thread->pThread);
    }
    else {
      pthread_join(thread->pThread, NULL);
    }
    thread->task = NULL;
  }
#elif defined(WIN32)
  if (thread->runnableFlag == true) {
    thread->runnableFlag = false;
    TerminateThread(thread->hThread, 0);
    WaitForSingleObject(thread->hThread, INFINITE);
  }
#else
  /* Workers poll runnableFlag and wake at least every receive timeout, so join is bounded. */
  thread->runnableFlag = false;
  if (thread->joinable) {
    thread->joinable = false;
    if (pthread_equal(thread->pThread, pthread_self())) {
      /* A worker stopping itself cannot join; let it release itself on exit. */
      pthread_detach(thread->pThread);
    }
    else {
      pthread_join(thread->pThread, NULL);
    }
  }
#endif

  return true;
}

/****************************************
 * uecho_thread_restart
 ****************************************/

bool uecho_thread_restart(uEchoThread* thread)
{
  uecho_thread_stop(thread);
  return uecho_thread_start(thread);
}

/****************************************
 * uecho_thread_isrunnable
 ****************************************/

bool uecho_thread_isrunnable(uEchoThread* thread)
{
  if (!thread)
    return false;

#if !defined(WIN32) && !defined(ESP_PLATFORM)
  pthread_testcancel();
#endif

  return thread->runnableFlag;
}

/****************************************
 * uecho_thread_isrunning
 ****************************************/

bool uecho_thread_isrunning(uEchoThread* thread)
{
  if (!thread)
    return false;

  return thread->runnableFlag;
}

/****************************************
 * uecho_thread_setaction
 ****************************************/

void uecho_thread_setaction(uEchoThread* thread, uEchoThreadFunc func)
{
  if (!thread)
    return;

  thread->action = func;
}

/****************************************
 * uecho_thread_setuserdata
 ****************************************/

void uecho_thread_setuserdata(uEchoThread* thread, void* value)
{
  if (!thread)
    return;

  thread->userData = value;
}

/****************************************
 * uecho_thread_getuserdata
 ****************************************/

void* uecho_thread_getuserdata(uEchoThread* thread)
{
  if (!thread)
    return NULL;

  return thread->userData;
}

/****************************************
 * uecho_sig_handler
 ****************************************/

#if !defined(WIN32) && !defined(ESP_PLATFORM)
static void uecho_sig_handler(int sign)
{
}
#endif
