/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller.h>

/****************************************
 * uecho_controller_new
 ****************************************/

uEchoController *uecho_controller_new()
{
	uEchoController *cp;

  cp = (uEchoController *)malloc(sizeof(uEchoController));

  if (!cp)
    return NULL;

  cp->mutex = uecho_mutex_new();
  cp->server = uecho_server_new();
  
  uecho_controller_setlasttid(cp, 0);
  
	return cp;
}

/****************************************
 * uecho_object_delete
 ****************************************/

void uecho_controller_delete(uEchoController *cp)
{
	uecho_controller_stop(cp);
	
	uecho_mutex_delete(cp->mutex);
  uecho_server_delete(cp->server);

  free(cp);
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoController *cp)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_server_start(cp->server);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_stop
 ****************************************/

bool uecho_controller_stop(uEchoController *cp)
{
  bool allActionsSucceeded = true;
  
  allActionsSucceeded &= uecho_server_stop(cp->server);
  
  return allActionsSucceeded;
}

/****************************************
 * uecho_controller_isrunning
 ****************************************/

bool uecho_controller_isrunning(uEchoController *cp)
{
  if (!uecho_server_isrunning(cp->server))
    return false;
	return true;
}

/****************************************
 * uecho_controller_getnexttid
 ****************************************/

uEchoTID uecho_controller_getnexttid(uEchoController *cp) {
  if (uEchoTidMax <= cp->lastTID) {
    cp->lastTID = 1;
  }
  else {
    cp->lastTID++;
  }
  return cp->lastTID;
}

/****************************************
 * uecho_controller_searchall
 ****************************************/

bool uecho_controller_sendsearchmessage(uEchoController *cp, uEchoMessage *msg) {
  uEchoObject *obj;
  uecho_message_settid(msg, uecho_controller_getnexttid(cp));

  obj = uecho_message_getsourceobject(msg);
  uecho_object_setcode(obj, uEchoNodeProfileObject);
  
  
  return true;
}

/****************************************
 * uecho_controller_searchall
 ****************************************/

bool uecho_controller_searchall(uEchoController *cp) {
  uEchoMessage *msg;
  
  msg = uecho_message_search_new();

  return uecho_controller_sendsearchmessage(cp, msg);
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoController *cp, byte objCode) {
  uEchoMessage *msg;
  uEchoObject *obj;
  uEchoProperty *prop;
  
  msg = uecho_message_search_new();
  
  obj = uecho_message_getdestinationobject(msg);
  uecho_object_setcode(obj, objCode);
  
  prop = uecho_message_getproperty(msg, 0);
  uecho_property_setcode(prop, uEchoOperatingStatus);
  uecho_property_setdata(prop, NULL, 0);
  
  return uecho_controller_sendsearchmessage(cp, msg);
}
