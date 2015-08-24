/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
