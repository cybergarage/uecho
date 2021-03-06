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
* uecho_object_property_observer_manager_new
****************************************/

uEchoObjectPropertyObserverManager* uecho_object_property_observer_manager_new(void)
{
  uEchoObjectPropertyObserverManager* obsMgr;

  obsMgr = (uEchoObjectPropertyObserverManager*)malloc(sizeof(uEchoObjectPropertyObserverManager));
  if (!obsMgr)
    return NULL;

  obsMgr->observers = uecho_object_property_observerlist_new();

  return obsMgr;
}

/****************************************
* uecho_object_property_observer_manager_delete
****************************************/

void uecho_object_property_observer_manager_delete(uEchoObjectPropertyObserverManager* obsMgr)
{
  uecho_object_property_observerlist_delete(obsMgr->observers);

  free(obsMgr);
}

/****************************************
 * uecho_object_property_observer_manager_setobserver
 ****************************************/

bool uecho_object_property_observer_manager_setobserver(uEchoObjectPropertyObserverManager* obsMgr, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestListener listener)
{
  uEchoObjectPropertyObserver* obs;

  obs = uecho_object_property_observer_manager_getobserver(obsMgr, esv, code);
  if (obs) {
    uecho_object_property_observer_remove(obs);
    uecho_object_property_observer_delete(obs);
  }

  obs = uecho_object_property_observer_new();
  if (!obs)
    return false;

  uecho_object_property_observer_setesv(obs, esv);
  uecho_object_property_observer_setpropetycode(obs, code);
  uecho_object_property_observer_setlistener(obs, listener);

  return uecho_object_property_observerlist_add(obsMgr->observers, obs);
}

/****************************************
 * uecho_object_property_observer_manager_getobservers
 ****************************************/

uEchoObjectPropertyObserver* uecho_object_property_observer_manager_getobservers(uEchoObjectPropertyObserverManager* obsMgr)
{
  return uecho_object_property_observerlist_gets(obsMgr->observers);
}

/****************************************
 * uecho_object_property_observer_manager_getobserver
 ****************************************/

uEchoObjectPropertyObserver* uecho_object_property_observer_manager_getobserver(uEchoObjectPropertyObserverManager* obsMgr, uEchoEsv esv, uEchoPropertyCode code)
{
  uEchoObjectPropertyObserver* obs;

  for (obs = uecho_object_property_observer_manager_getobservers(obsMgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (esv != uecho_object_property_observer_getesv(obs))
      continue;
    if (code == uecho_object_property_observer_getpropetycode(obs))
      return obs;
  }

  return NULL;
}
