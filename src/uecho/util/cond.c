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

  pthread_mutex_init(&cond->mutex_id, NULL);
  pthread_cond_init(&cond->cond_id, NULL);

  return cond;
}

/****************************************
 * uecho_cond_delete
 ****************************************/

bool uecho_cond_delete(uEchoCond* cond)
{
  if (!cond)
    return false;

  pthread_mutex_destroy(&cond->mutex_id);
  pthread_cond_destroy(&cond->cond_id);

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

  pthread_cond_wait(&cond->cond_id, &cond->mutex_id);

  return true;
}

/****************************************
 * uecho_cond_timedwait
 ****************************************/

bool uecho_cond_timedwait(uEchoCond* cond)
{
  if (!cond)
    return false;

  pthread_cond_wait(&cond->cond_id, &cond->mutex_id);

  return true;
}

/****************************************
 * uecho_cond_signal
 ****************************************/

bool uecho_cond_signal(uEchoCond* cond)
{
  if (!cond)
    return false;

  pthread_cond_signal(&cond->cond_id);

  return true;
}
