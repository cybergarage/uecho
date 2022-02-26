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

uEchoMutex* uecho_mutex_new(void)
{
  uEchoMutex* mutex;

  mutex = (uEchoMutex*)malloc(sizeof(uEchoMutex));

  if (!mutex)
    return NULL;

#if defined(WIN32)
  mutex->mutex_id = CreateMutex(NULL, false, NULL);
#else
  pthread_mutex_init(&mutex->mutex_id, NULL);
#endif

  return mutex;
}

/****************************************
* uecho_mutex_delete
****************************************/

bool uecho_mutex_delete(uEchoMutex* mutex)
{
  if (!mutex)
    return false;

#if defined(WIN32)
  CloseHandle(mutex->mutex_id);
#else
  pthread_mutex_destroy(&mutex->mutex_id);
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
  WaitForSingleObject(mutex->mutex_id, INFINITE);
#else
  pthread_mutex_lock(&mutex->mutex_id);
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
  ReleaseMutex(mutex->mutex_id);
#else
  pthread_mutex_unlock(&mutex->mutex_id);
#endif
  return true;
}
