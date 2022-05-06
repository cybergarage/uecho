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
* uecho_message_observer_new
****************************************/

uEchoMessageObserver* uecho_message_observer_new(void)
{
  uEchoMessageObserver* obs;

  obs = (uEchoMessageObserver*)malloc(sizeof(uEchoMessageObserver));

  if (!obs)
    return NULL;

  uecho_list_node_init((uEchoList*)obs);

  uecho_message_observer_setobject(obs, NULL);
  uecho_message_observer_sethandler(obs, NULL);

  return obs;
}

/****************************************
* uecho_message_observer_delete
****************************************/

void uecho_message_observer_delete(uEchoMessageObserver* obs)
{
  uecho_list_remove((uEchoList*)obs);

  free(obs);
}

/****************************************
 * uecho_message_observer_setobject
 ****************************************/

void uecho_message_observer_setobject(uEchoMessageObserver* obs, void* obj)
{
  obs->object = obj;
}

/****************************************
 * uecho_message_observer_getobjcet
 ****************************************/

void* uecho_message_observer_getobjcet(uEchoMessageObserver* obs)
{
  return obs->object;
}

/****************************************
 * uecho_message_observer_sethandler
 ****************************************/

void uecho_message_observer_sethandler(uEchoMessageObserver* obs, uEchoMessageHandler handler)
{
  obs->handler = handler;
}

/****************************************
 * uecho_message_observer_getlistener
 ****************************************/

uEchoMessageHandler uecho_message_observer_gethandler(uEchoMessageObserver* obs)
{
  return obs->handler;
}
