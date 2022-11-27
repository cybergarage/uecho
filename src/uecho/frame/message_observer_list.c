/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/frame/message.h>

/****************************************
 * uecho_message_observerlist_new
 ****************************************/

uEchoMessageObserverList* uecho_message_observerlist_new(void)
{
  uEchoMessageObserverList* observers;

  observers = (uEchoMessageObserverList*)malloc(sizeof(uEchoMessageObserverList));
  if (!observers)
    return NULL;

  uecho_list_header_init((uEchoList*)observers);

  return observers;
}

/****************************************
 * uecho_message_observerlist_delete
 ****************************************/

void uecho_message_observerlist_delete(uEchoMessageObserverList* observers)
{
  uecho_message_observerlist_clear(observers);

  free(observers);
}
