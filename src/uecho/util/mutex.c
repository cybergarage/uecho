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

#if defined(WIN32) && !defined(ITRON)
  mutex->mutexID = CreateMutex(NULL, false, NULL);
#elif defined(BTRON)
  mutex->mutexID = cre_sem(1, SEM_EXCL);
#elif defined(ITRON) 
  T_CSEM	csem;
  csem.sematr = TA_TFIFO;
  csem.isemcnt = 1;
  csem.maxsem = 1;
  csem.name = NULL;
  mutex->mutexID = acre_sem(&csem);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
  T_CSEM	csem;
  csem.exinf = 0;
  csem.sematr = TA_TFIFO | TA_FIRST;
  csem.isemcnt = 0;
  csem.maxsem = 1;
  mutex->mutexID = tk_cre_sem(&csem);
#elif defined(TENGINE) && defined(PROCESS_BASE)
  mutex->mutexID = b_cre_sem(1, SEM_EXCL);
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

#if defined(WIN32) && !defined(ITRON)
	CloseHandle(mutex->mutexID);
#elif defined(BTRON)
	del_sem(mutex->mutexID);
#elif defined(ITRON)
	del_sem(mutex->mutexID);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	tk_del_sem(mutex->mutexID);
#elif defined(TENGINE) && defined(PROCESS_BASE)
	b_del_sem(mutex->mutexID);
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

#if defined(WIN32) && !defined(ITRON)
	WaitForSingleObject(mutex->mutexID, INFINITE);
#elif defined(BTRON)
	wai_sem(mutex->mutexID, T_FOREVER);
#elif defined(ITRON)
	twai_sem(mutex->mutexID, TMO_FEVR);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	tk_wai_sem(mutex->mutexID, 1, TMO_FEVR);
#elif defined(TENGINE) && defined(PROCESS_BASE)
	b_wai_sem(mutex->mutexID, T_FOREVER);
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

#if defined(WIN32) && !defined(ITRON)
	ReleaseMutex(mutex->mutexID);
#elif defined(BTRON)
	sig_sem(mutex->mutexID);
#elif defined(ITRON)
	sig_sem(mutex->mutexID);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
	tk_sig_sem(mutex->mutexID, 1);
#elif defined(TENGINE) && defined(PROCESS_BASE)
	b_sig_sem(mutex->mutexID);
#else
	pthread_mutex_unlock(&mutex->mutexID);
#endif
	return true;
}
