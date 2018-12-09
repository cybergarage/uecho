/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/observer.h>

/****************************************
* uecho_object_property_observerlist_new
****************************************/

uEchoObjectPropertyObserverList* uecho_object_property_observerlist_new(void)
{
  uEchoObjectPropertyObserverList* observers;

  observers = (uEchoObjectPropertyObserverList*)malloc(sizeof(uEchoObjectPropertyObserverList));
  if (!observers)
    return NULL;

  uecho_list_header_init((uEchoList*)observers);

  return observers;
}

/****************************************
* uecho_object_property_observerlist_delete
****************************************/

void uecho_object_property_observerlist_delete(uEchoObjectPropertyObserverList* observers)
{
  uecho_object_property_observerlist_clear(observers);

  free(observers);
}
