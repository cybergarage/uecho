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

/****************************************
* uecho_threadlist_new
****************************************/

uEchoThreadList* uecho_threadlist_new(void)
{
  uEchoThreadList* thread_list;

  thread_list = (uEchoThreadList*)malloc(sizeof(uEchoThreadList));

  if (!thread_list)
    return NULL;

  uecho_list_header_init((uEchoList*)thread_list);
  thread_list->runnable_flag = false;
  thread_list->action = NULL;
  thread_list->user_data = NULL;

  return thread_list;
}

/****************************************
* uecho_threadlist_delete
****************************************/

void uecho_threadlist_delete(uEchoThreadList* thread_list)
{
  if (!thread_list)
    return;

  uecho_threadlist_clear(thread_list);
  free(thread_list);
}

/****************************************
* uecho_threadlist_start
****************************************/

bool uecho_threadlist_start(uEchoThreadList* thread_list)
{
  uEchoThreadList* thread;

  if (!thread_list)
    return false;

  for (thread = uecho_threadlist_gets(thread_list); thread != NULL; thread = uecho_thread_next(thread)) {
    uecho_thread_start(thread);
  }

  return true;
}

/****************************************
* uecho_threadlist_stop
****************************************/

bool uecho_threadlist_stop(uEchoThreadList* thread_list)
{
  uEchoThreadList* thread;

  if (!thread_list)
    return false;

  for (thread = uecho_threadlist_gets(thread_list); thread != NULL; thread = uecho_thread_next(thread))
    uecho_thread_stop(thread);

  return true;
}
