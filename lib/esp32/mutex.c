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

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/****************************************
 * uecho_mutex_new
 ****************************************/

uEchoMutex* uecho_mutex_new(void)
{
  uEchoMutex* mutex;

  mutex = (uEchoMutex*)malloc(sizeof(uEchoMutex));

  if (!mutex)
    return NULL;

  mutex->mutex = xSemaphoreCreateMutex();
  if (mutex->mutex == NULL) {
    free(mutex);
    return NULL;
  }

  return mutex;
}

/****************************************
 * uecho_mutex_delete
 ****************************************/

bool uecho_mutex_delete(uEchoMutex* mutex)
{
  if (!mutex)
    return false;

  vSemaphoreDelete(mutex->mutex);
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

  if (xSemaphoreTake(mutex->mutex, portMAX_DELAY) == pdTRUE) {
    return true;
  }

  return false;
}

/****************************************
 * uecho_mutex_unlock
 ****************************************/

bool uecho_mutex_unlock(uEchoMutex* mutex)
{
  if (!mutex)
    return false;

  if (xSemaphoreGive(mutex->mutex) == pdTRUE) {
    return true;
  }

  return false;
}
