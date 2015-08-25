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
#include <uecho/profile.h>

/****************************************
* uecho_message_search_new
****************************************/

uEchoMessage *uecho_message_search_new(void)
{
  uEchoMessage *msg;
  uEchoProperty *prop;
  
	msg = uecho_message_new();

  if (!msg)
    return NULL;

  uecho_message_setesv(msg, uEchoEsvReadRequest);
  
  uecho_message_setsourceobjectcode(msg, uEchoNodeProfileObject);
  uecho_message_setdestinationobjectcode(msg, uEchoNodeProfileObject);
  
  prop = uecho_property_new();
  uecho_property_setcode(prop, uEchoNodeProfileClassSelfNodeInstanceListS);
  uecho_property_setdata(prop, NULL, 0);
  uecho_message_addproperty(msg, prop);
  
	return msg;
}

/****************************************
 * uecho_message_issearchrequest
 ****************************************/

bool uecho_message_issearchrequest(uEchoMessage *msg) {
  uEchoProperty *prop;
  
  if (uecho_message_getesv(msg) != uEchoEsvReadRequest)
    return false;

  if (uecho_message_getopc(msg) != 1)
    return false;

  prop = uecho_message_getproperty(msg, 0);
  if (uecho_property_getcode(prop) != uEchoNodeProfileClassSelfNodeInstanceListS)
    return false;
  
  return true;
}
