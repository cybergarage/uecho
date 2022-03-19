/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/frame/property.h>

/****************************************
* uecho_message_observer_new
****************************************/

uEchoObjectPropertyObserver* uecho_object_property_observer_new(void)
{
  uEchoObjectPropertyObserver* obs;

  obs = (uEchoObjectPropertyObserver*)malloc(sizeof(uEchoObjectPropertyObserver));

  if (!obs)
    return NULL;

  uecho_list_node_init((uEchoList*)obs);

  uecho_object_property_observer_setpropetycode(obs, 0);
  uecho_object_property_observer_sethandler(obs, NULL);

  return obs;
}

/****************************************
* uecho_message_observer_delete
****************************************/

void uecho_object_property_observer_delete(uEchoObjectPropertyObserver* obs)
{
  uecho_list_remove((uEchoList*)obs);

  free(obs);
}

/****************************************
 * uecho_object_property_observer_setesv
 ****************************************/

void uecho_object_property_observer_setesv(uEchoObjectPropertyObserver* obs, uEchoEsv esv)
{
  obs->esv = esv;
}

/****************************************
 * uecho_object_property_observer_getesv
 ****************************************/

uEchoEsv uecho_object_property_observer_getesv(uEchoObjectPropertyObserver* obs)
{
  return obs->esv;
}

/****************************************
 * uecho_object_property_observer_setpropetycode
 ****************************************/

void uecho_object_property_observer_setpropetycode(uEchoObjectPropertyObserver* obs, uEchoPropertyCode code)
{
  obs->prop_code = code;
}

/****************************************
 * uecho_object_property_observer_getpropetycode
 ****************************************/

uEchoPropertyCode uecho_object_property_observer_getpropetycode(uEchoObjectPropertyObserver* obs)
{
  return obs->prop_code;
}

/****************************************
 * uecho_object_property_observer_sethandler
 ****************************************/

void uecho_object_property_observer_sethandler(uEchoObjectPropertyObserver* obs, uEchoPropertyRequestHandler handler)
{
  obs->handler = handler;
}

/****************************************
 * uecho_object_property_observer_getlistener
 ****************************************/

uEchoPropertyRequestHandler uecho_object_property_observer_getlistener(uEchoObjectPropertyObserver* obs)
{
  return obs->handler;
}
