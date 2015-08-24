/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/object.h>

/****************************************
* uecho_object_property_observer_manager_new
****************************************/

uEchoObjectPropertyObserverManager *uecho_object_property_observer_manager_new(void)
{
	uEchoObjectPropertyObserverManager *obsMgr;

	obsMgr = (uEchoObjectPropertyObserverManager *)malloc(sizeof(uEchoObjectPropertyObserverManager));
  if (!obsMgr)
    return NULL;

  uecho_list_header_init((uEchoList *)obsMgr);

	return obsMgr;
}

/****************************************
* uecho_object_property_observer_manager_delete
****************************************/

void uecho_object_property_observer_manager_delete(uEchoObjectPropertyObserverManager *obsMgr)
{
	uecho_object_property_observer_manager_clear(obsMgr);

	free(obsMgr);
}

/****************************************
 * uecho_object_property_observer_manager_setobserver
 ****************************************/

bool uecho_object_property_observer_manager_setobserver(uEchoObjectPropertyObserverManager *obsMgr, uEchoPropertyCode code, uEchoObjectMessageListener listener)
{
  uEchoObjectPropertyObserver *obs;
  
  obs = uecho_object_property_observer_manager_getobserver(obsMgr, code);
  if (obs) {
    uecho_object_property_observer_remove(obs);
    uecho_object_property_observer_delete(obs);
  }
  
  obs = uecho_object_property_observer_new();
  if (!obs)
    return false;
  
  uecho_object_property_observer_setpropetycode(obs, code);
  uecho_object_property_observer_setlistener(obs, listener);
  
  return uecho_object_property_observer_manager_addobserver(obsMgr, obs);
}

/****************************************
 * uecho_object_property_observer_manager_getobserver
 ****************************************/

uEchoObjectPropertyObserver *uecho_object_property_observer_manager_getobserver(uEchoObjectPropertyObserverManager *obsMgr, uEchoPropertyCode code)
{
  uEchoObjectPropertyObserver *obs;
  
  for (obs = uecho_object_property_observer_manager_getobservers(obsMgr); obs; obs = uecho_object_property_observer_next(obs)) {
    if (code == uecho_object_property_observer_getpropetycode(obs))
      return obs;
  }

  return NULL;
}
