/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/cond.h>

#include <errno.h>

/****************************************
 * uecho_cond_new
 ****************************************/

uEchoCond* uecho_cond_new(void)
{
  uEchoCond* cond;

  cond = (uEchoCond*)malloc(sizeof(uEchoCond));

  if (!cond)
    return NULL;

  pthread_mutex_init(&cond->mutexId, NULL);
  pthread_cond_init(&cond->condId, NULL);

  return cond;
}

/****************************************
 * uecho_cond_delete
 ****************************************/

bool uecho_cond_delete(uEchoCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_destroy(&cond->mutexId);
  pthread_cond_destroy(&cond->condId);

  free(cond);

  return true;
}

/****************************************
 * uecho_cond_wait
 ****************************************/

bool uecho_cond_wait(uEchoCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutexId);
  pthread_cond_wait(&cond->condId, &cond->mutexId);
  pthread_mutex_unlock(&cond->mutexId);

  return true;
}

/****************************************
 * uecho_cond_timedwait
 ****************************************/

bool uecho_cond_timedwait(uEchoCond* cond, clock_t mtime)
{
  struct timespec to;
  int c;

  if (!cond)
    return false;

  to.tv_sec = time(NULL) + (mtime / CLOCKS_PER_SEC);
  to.tv_nsec = 0;

  pthread_mutex_lock(&cond->mutexId);
  c = pthread_cond_timedwait(&cond->condId, &cond->mutexId, &to);
  pthread_mutex_unlock(&cond->mutexId);

  return (c == 0) ? true : false;
}

/****************************************
 * uecho_cond_signal
 ****************************************/

bool uecho_cond_signal(uEchoCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_lock(&cond->mutexId);
  pthread_cond_signal(&cond->condId);
  pthread_mutex_unlock(&cond->mutexId);

  return true;
}

/****************************************
 * uecho_cond_lock
 ****************************************/

bool uecho_cond_lock(uEchoCond* cond)
{
  if (!cond)
    return false;

  return (pthread_mutex_lock(&cond->mutexId) == 0) ? true : false;
}

/****************************************
 * uecho_cond_unlock
 ****************************************/

bool uecho_cond_unlock(uEchoCond* cond)
{
  if (!cond)
    return false;

  return (pthread_mutex_unlock(&cond->mutexId) == 0) ? true : false;
}

/****************************************
 * uecho_cond_getdeadline
 ****************************************/

bool uecho_cond_getdeadline(clock_t mtime, struct timespec* deadline)
{
  long long nsec;

  if (!deadline)
    return false;

  if (clock_gettime(CLOCK_REALTIME, deadline) != 0)
    return false;

  /* mtime is in clock ticks (CLOCKS_PER_SEC per second). */
  nsec = (long long)deadline->tv_nsec + ((long long)mtime % CLOCKS_PER_SEC) * (1000000000LL / CLOCKS_PER_SEC);
  deadline->tv_sec += (time_t)(mtime / CLOCKS_PER_SEC) + (time_t)(nsec / 1000000000LL);
  deadline->tv_nsec = (long)(nsec % 1000000000LL);

  return true;
}

/****************************************
 * uecho_cond_waituntil
 ****************************************/

bool uecho_cond_waituntil(uEchoCond* cond, const struct timespec* deadline)
{
  if (!cond || !deadline)
    return false;

  /* The caller holds the lock; a spurious wakeup also returns true, so re-check the predicate. */
  return (pthread_cond_timedwait(&cond->condId, &cond->mutexId, deadline) == 0) ? true : false;
}
