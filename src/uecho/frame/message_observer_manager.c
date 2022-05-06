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
* uecho_message_observer_manager_new
****************************************/

uEchoMessageObserverManager* uecho_message_observer_manager_new(void)
{
  uEchoMessageObserverManager* mgr;

  mgr = (uEchoMessageObserverManager*)malloc(sizeof(uEchoMessageObserverManager));
  if (!mgr)
    return NULL;

  mgr->observers = uecho_message_observerlist_new();

  return mgr;
}

/****************************************
* uecho_message_observer_manager_delete
****************************************/

void uecho_message_observer_manager_delete(uEchoMessageObserverManager* mgr)
{
  uecho_message_observerlist_delete(mgr->observers);

  free(mgr);
}

/****************************************
 * uecho_message_observer_manager_setobserver
 ****************************************/

bool uecho_message_observer_manager_addobserver(uEchoMessageObserverManager* mgr, void* obj, uEchoMessageHandler handler)
{
  uEchoMessageObserver* obs;

  obs = uecho_message_observer_new();
  if (!obs)
    return false;

  uecho_message_observer_setobject(obs, obj);
  uecho_message_observer_sethandler(obs, handler);

  return uecho_message_observerlist_add(mgr->observers, obs);
}

/****************************************
 * uecho_message_observer_manager_perform
 ****************************************/

bool uecho_message_observer_manager_perform(uEchoMessageObserverManager* mgr, uEchoMessage* msg)
{
  uEchoMessageObserver* obs;
  uEchoMessageHandler handler;
  void* obj;

  for (obs = uecho_message_observer_manager_getobservers(mgr); obs; obs = uecho_message_observer_next(obs)) {
    handler = uecho_message_observer_gethandler(obs);
    obj = uecho_message_observer_getobjcet(obs);
    if (!handler || !obj)
      continue;
    handler(obj, msg);
  }

  return true;
}
