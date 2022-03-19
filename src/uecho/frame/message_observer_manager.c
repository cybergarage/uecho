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
  uEchoMessageObserverManager* obs_mgr;

  obs_mgr = (uEchoMessageObserverManager*)malloc(sizeof(uEchoMessageObserverManager));
  if (!obs_mgr)
    return NULL;

  obs_mgr->observers = uecho_message_observerlist_new();

  return obs_mgr;
}

/****************************************
* uecho_message_observer_manager_delete
****************************************/

void uecho_message_observer_manager_delete(uEchoMessageObserverManager* obs_mgr)
{
  uecho_message_observerlist_delete(obs_mgr->observers);

  free(obs_mgr);
}

/****************************************
 * uecho_message_observer_manager_setobserver
 ****************************************/

bool uecho_message_observer_manager_addobserver(uEchoMessageObserverManager* obs_mgr, void *obj, uEchoMessageHandler handler)
{
  uEchoMessageObserver* obs;

  obs = uecho_message_observer_new();
  if (!obs)
    return false;

  uecho_message_observer_setobject(obs, obj);
  uecho_message_observer_sethandler(obs, handler);

  return uecho_message_observerlist_add(obs_mgr->observers, obs);
}

/****************************************
 * uecho_message_observer_manager_getobservers
 ****************************************/

uEchoMessageObserver* uecho_message_observer_manager_getobservers(uEchoMessageObserverManager* obs_mgr)
{
  return uecho_message_observerlist_gets(obs_mgr->observers);
}
