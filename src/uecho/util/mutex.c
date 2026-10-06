/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/mutex.h>

#include <errno.h>

/****************************************
 * uecho_mutex_new
 ****************************************/

static uEchoMutex* uecho_mutex_newwithtype(bool recursive)
{
  uEchoMutex* mutex;

  mutex = (uEchoMutex*)malloc(sizeof(uEchoMutex));

  if (!mutex)
    return NULL;

#if defined(WIN32)
  mutex->mutexId = CreateMutex(NULL, false, NULL);
#else
  pthread_mutexattr_t attr;
  if (pthread_mutexattr_init(&attr) != 0) {
    free(mutex);
    return NULL;
  }
  if (recursive && pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
    pthread_mutexattr_destroy(&attr);
    free(mutex);
    return NULL;
  }
  int result = pthread_mutex_init(&mutex->mutexId, &attr);
  pthread_mutexattr_destroy(&attr);
  if (result != 0) {
    free(mutex);
    return NULL;
  }
#endif

  return mutex;
}

uEchoMutex* uecho_mutex_new(void)
{
  return uecho_mutex_newwithtype(false);
}

uEchoMutex* uecho_mutex_newrecursive(void)
{
  return uecho_mutex_newwithtype(true);
}

/****************************************
 * uecho_mutex_delete
 ****************************************/

bool uecho_mutex_delete(uEchoMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  CloseHandle(mutex->mutexId);
#else
  pthread_mutex_destroy(&mutex->mutexId);
#endif
  free(mutex);

  return true;
}

/****************************************
 * uecho_mutex_lock
 ****************************************/

bool uecho_mutex_lock(uEchoMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  WaitForSingleObject(mutex->mutexId, INFINITE);
#else
  pthread_mutex_lock(&mutex->mutexId);
#endif

  return true;
}

/****************************************
 * uecho_mutex_unlock
 ****************************************/

bool uecho_mutex_unlock(uEchoMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  ReleaseMutex(mutex->mutexId);
#else
  pthread_mutex_unlock(&mutex->mutexId);
#endif
  return true;
}
