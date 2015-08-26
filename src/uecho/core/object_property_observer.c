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
* uecho_obsect_property_observer_new
****************************************/

uEchoObjectPropertyObserver *uecho_object_property_observer_new(void)
{
	uEchoObjectPropertyObserver *obs;

	obs = (uEchoObjectPropertyObserver *)malloc(sizeof(uEchoObjectPropertyObserver));
    
  if (!obs)
    return NULL;

  uecho_list_node_init((uEchoList *)obs);
  
  uecho_object_property_observer_setpropetycode(obs, 0);
  uecho_object_property_observer_setlistener(obs, NULL);
  
  return obs;
}

/****************************************
* uecho_obsect_property_observer_delete
****************************************/

void uecho_object_property_observer_delete(uEchoObjectPropertyObserver *obs)
{
	uecho_list_remove((uEchoList *)obs);
  
  free(obs);
}

/****************************************
 * uecho_object_property_observer_setpropetycode
 ****************************************/

void uecho_object_property_observer_setpropetycode(uEchoObjectPropertyObserver *obs, uEchoPropertyCode code)
{
  obs->propCode = code;
}

/****************************************
 * uecho_object_property_observer_getpropetycode
 ****************************************/

uEchoPropertyCode uecho_object_property_observer_getpropetycode(uEchoObjectPropertyObserver *obs)
{
  return obs->propCode;
}

/****************************************
 * uecho_object_property_observer_setlistener
 ****************************************/

void uecho_object_property_observer_setlistener(uEchoObjectPropertyObserver *obs, uEchoObjectMessageListener listener)
{
  obs->listener = listener;
}

/****************************************
 * uecho_object_property_observer_getlistener
 ****************************************/

uEchoObjectMessageListener uecho_object_property_observer_getlistener(uEchoObjectPropertyObserver *obs)
{
  return obs->listener;
}
