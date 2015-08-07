/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/core/message.h>

/****************************************
* uecho_message_search_new
****************************************/

uEchoMessage *uecho_message_search_new()
{
  uEchoMessage *msg;
  uEchoObject *obj;
  uEchoProperty *prop;
  
	msg = uecho_message_new();

  if (!msg)
    return NULL;

  obj = uecho_message_getdestinationobject(msg);
  uecho_object_setcode(obj, uEchoNodeProfileObject);
  
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setopc(msg, 1);
  
  prop = uecho_message_getproperty(msg, 0);
  uecho_property_setcode(prop, uEchoSelfNodeInstanceListS);
  uecho_property_setdata(prop, NULL, 0);
  
	return msg;
}
