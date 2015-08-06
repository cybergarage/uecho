/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/mutex.h>

#if !defined(WINCE)
#include <errno.h>
#endif

/****************************************
* uecho_mutex_new
****************************************/

uEchoMutex *uecho_mutex_new()
{
	uEchoMutex *mutex;

  mutex = (uEchoMutex *)malloc(sizeof(uEchoMutex));

  if (!mutex)
    return NULL;

#if defined(WIN32)
  mutex->mutexID = CreateMutex(NULL, false, NULL);
#else
  pthread_mutex_init(&mutex->mutexID, NULL);
#endif

	return mutex;
}

/****************************************
* uecho_mutex_delete
****************************************/

bool uecho_mutex_delete(uEchoMutex *mutex)
{
	if (!mutex)
		return false;

#if defined(WIN32)
	CloseHandle(mutex->mutexID);
#else
	pthread_mutex_destroy(&mutex->mutexID);
#endif
	free(mutex);

	return true;
}

/****************************************
* uecho_mutex_lock
****************************************/

bool uecho_mutex_lock(uEchoMutex *mutex)
{
	if (!mutex)
		return false;

#if defined(WIN32)
	WaitForSingleObject(mutex->mutexID, INFINITE);
#else
	pthread_mutex_lock(&mutex->mutexID);
#endif

	return true;
}

/****************************************
* uecho_mutex_unlock
****************************************/

bool uecho_mutex_unlock(uEchoMutex *mutex)
{
	if (!mutex)
		return false;

#if defined(WIN32)
	ReleaseMutex(mutex->mutexID);
#else
	pthread_mutex_unlock(&mutex->mutexID);
#endif
	return true;
}
