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

uEchoThreadList *uecho_threadlist_new(void)
{
  uEchoThreadList *threadList;

  threadList = (uEchoThreadList *)malloc(sizeof(uEchoThreadList));

  if (!threadList)
    return NULL;

  uecho_list_header_init((uEchoList *)threadList);
  threadList->runnableFlag = false;
  threadList->action = NULL;
  threadList->userData = NULL;

  return threadList;
}

/****************************************
* uecho_threadlist_delete
****************************************/

void uecho_threadlist_delete(uEchoThreadList *threadList)
{
  if (!threadList)
    return;
  
  uecho_threadlist_clear(threadList);
  free(threadList);
}

/****************************************
* uecho_threadlist_start
****************************************/

bool uecho_threadlist_start(uEchoThreadList *threadList)
{
  uEchoThreadList *thread;

  if (!threadList)
    return false;
  
  for (thread = uecho_threadlist_gets(threadList); thread != NULL; thread = uecho_thread_next(thread)) {
    uecho_thread_start(thread);
  }

  return true;
}

/****************************************
* uecho_threadlist_stop
****************************************/

bool uecho_threadlist_stop(uEchoThreadList *threadList)
{
  uEchoThreadList *thread;
  
  if (!threadList)
    return false;
  
  for (thread = uecho_threadlist_gets(threadList); thread != NULL; thread = uecho_thread_next(thread))
    uecho_thread_stop(thread);

  return true;
}
