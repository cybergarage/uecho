/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/observer.h>

/****************************************
* uecho_object_property_observer_manager_new
****************************************/

uEchoObjectPropertyObserverManager* uecho_object_property_observer_manager_new(void)
{
  uEchoObjectPropertyObserverManager* obs_mgr;

  obs_mgr = (uEchoObjectPropertyObserverManager*)malloc(sizeof(uEchoObjectPropertyObserverManager));
  if (!obs_mgr)
    return NULL;

  obs_mgr->observers = uecho_object_property_observerlist_new();

  return obs_mgr;
}

/****************************************
* uecho_object_property_observer_manager_delete
****************************************/

void uecho_object_property_observer_manager_delete(uEchoObjectPropertyObserverManager* obs_mgr)
{
  uecho_object_property_observerlist_delete(obs_mgr->observers);

  free(obs_mgr);
}

/****************************************
 * uecho_object_property_observer_manager_setobserver
 ****************************************/

bool uecho_object_property_observer_manager_setobserver(uEchoObjectPropertyObserverManager* obs_mgr, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestHandler handler)
{
  uEchoObjectPropertyObserver* obs;

  obs = uecho_object_property_observer_manager_getobserver(obs_mgr, esv, code);
  if (obs) {
    uecho_object_property_observer_remove(obs);
    uecho_object_property_observer_delete(obs);
  }

  obs = uecho_object_property_observer_new();
  if (!obs)
    return false;

  uecho_object_property_observer_setesv(obs, esv);
  uecho_object_property_observer_setpropetycode(obs, code);
  uecho_object_property_observer_sethandler(obs, handler);

  return uecho_object_property_observerlist_add(obs_mgr->observers, obs);
}

/****************************************
 * uecho_object_property_observer_manager_getobservers
 ****************************************/

uEchoObjectPropertyObserver* uecho_object_property_observer_manager_getobservers(uEchoObjectPropertyObserverManager* obs_mgr)
{
  return uecho_object_property_observerlist_gets(obs_mgr->observers);
}

/****************************************
 * uecho_object_property_observer_manager_getobserver
 ****************************************/

uEchoObjectPropertyObserver* uecho_object_property_observer_manager_getobserver(uEchoObjectPropertyObserverManager* obs_mgr, uEchoEsv esv, uEchoPropertyCode code)
{
  uEchoObjectPropertyObserver* obs;

  for (obs = uecho_object_property_observer_manager_getobservers(obs_mgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (esv != uecho_object_property_observer_getesv(obs))
      continue;
    if (code == uecho_object_property_observer_getpropetycode(obs))
      return obs;
  }

  return NULL;
}
