/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/util/thread.h>
#include <uecho/util/timer.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/****************************************
 * Thread Function
 ****************************************/

static void uecho_freertos_thread_proc(void* param)
{
  uEchoThread* thread = (uEchoThread*)param;
  if (thread->action != NULL) {
    thread->action(thread);
  }
  vTaskDelete(NULL);
}

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
  thread->task = NULL;

  return thread;
}

/****************************************
 * uecho_thread_delete
 ****************************************/

bool uecho_thread_delete(uEchoThread* thread)
{
  if (!thread)
    return false;

  if (thread->runnableFlag == true) {
    uecho_thread_stop(thread);
  }

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

  BaseType_t ret = xTaskCreate(
      uecho_freertos_thread_proc,
      "uecho_thread",
      UECHO_THREAD_STACK_SIZE,
      thread,
      UECHO_THREAD_PRIORITY,
      &thread->task);

  if (ret != pdPASS) {
    thread->runnableFlag = false;
    return false;
  }

  return true;
}

/****************************************
 * uecho_thread_stop
 ****************************************/

bool uecho_thread_stop(uEchoThread* thread)
{
  if (!thread)
    return false;

  if (thread->runnableFlag == true) {
    thread->runnableFlag = false;
    if (thread->task != NULL) {
      vTaskDelete(thread->task);
      thread->task = NULL;
    }
  }

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
