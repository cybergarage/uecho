/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/controller.h>

/****************************************
 * uecho_controller_new
 ****************************************/

uEchoControlPoint *uecho_controller_new()
{
	uEchoControlPoint *cp;

  cp = (uEchoControlPoint *)malloc(sizeof(uEchoControlPoint));

  if (!cp)
    return NULL;

  cp->mutex = uecho_mutex_new();

  uecho_controller_setlasttid(cp, 0);
  
	return cp;
}

/****************************************
 * uecho_object_delete
 ****************************************/

void uecho_controller_delete(uEchoControlPoint *cp)
{
	uecho_controller_stop(cp);
	
	uecho_mutex_delete(cp->mutex);

    free(cp);
}

/****************************************
 * uecho_controller_start
 ****************************************/

bool uecho_controller_start(uEchoControlPoint *cp)
{
	return true;
}

/****************************************
 * uecho_controller_stop
 ****************************************/

bool uecho_controller_stop(uEchoControlPoint *cp)
{
	return true;
}

/****************************************
 * uecho_controller_isrunning
 ****************************************/

bool uecho_controller_isrunning(uEchoControlPoint *cp)
{
	return true;
}

/****************************************
 * uecho_controller_getnexttid
 ****************************************/

uEchoTID uecho_controller_getnexttid(uEchoControlPoint *cp) {
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

bool uecho_controller_searchall(uEchoControlPoint *cp) {
    byte nodeProfileObj[3] = {0x0E, 0xF0, 0x01};
    return true;
}

/****************************************
 * uecho_controller_searchobject
 ****************************************/

bool uecho_controller_searchobject(uEchoControlPoint *cp) {
    return true;
}
