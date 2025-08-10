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
#include <uecho/util/mutex.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/****************************************
 * uecho_cond_new
 ****************************************/

uEchoCond* uecho_cond_new(void)
{
  uEchoCond* cond;

  cond = (uEchoCond*)malloc(sizeof(uEchoCond));

  if (!cond)
    return NULL;

  cond->sem = xSemaphoreCreateBinary();
  if (cond->sem == NULL) {
    free(cond);
    return NULL;
  }

  return cond;
}

/****************************************
 * uecho_cond_delete
 ****************************************/

bool uecho_cond_delete(uEchoCond* cond)
{
  if (!cond)
    return false;

  vSemaphoreDelete(cond->sem);
  free(cond);

  return true;
}

/****************************************
 * uecho_cond_wait
 ****************************************/

bool uecho_cond_wait(uEchoCond* cond, uEchoMutex* mutex, uint32_t timeout_ms)
{
  if (!cond || !mutex)
    return false;

  // Unlock the mutex before waiting
  uecho_mutex_unlock(mutex);

  TickType_t ticks_to_wait = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
  bool ret = (xSemaphoreTake(cond->sem, ticks_to_wait) == pdTRUE);

  // Re-lock the mutex after waking up
  uecho_mutex_lock(mutex);

  return ret;
}

/****************************************
 * uecho_cond_signal
 ****************************************/

bool uecho_cond_signal(uEchoCond* cond)
{
  if (!cond)
    return false;

  return (xSemaphoreGive(cond->sem) == pdTRUE);
}
