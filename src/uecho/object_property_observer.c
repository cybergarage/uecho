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
* uecho_obsect_property_observer_new
****************************************/

uEchoObjectPropertyObserver *uecho_obsect_property_observer_new(void)
{
	uEchoObjectPropertyObserver *obs;

	obs = (uEchoObjectPropertyObserver *)malloc(sizeof(uEchoObjectPropertyObserver));
    
  if (!obs)
    return NULL;

  uecho_list_node_init((uEchoList *)obs);
  
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
