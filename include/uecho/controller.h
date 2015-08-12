/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONTROLLER_H_
#define _UECHO_CONTROLLER_H_

#include <uecho/typedef.h>
#include <uecho/const.h>
#include <uecho/util/mutex.h>
#include <uecho/core/server.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Data Type
****************************************/

typedef struct _uEchoController {
	uEchoMutex *mutex;
  uEchoServer *server;
  uEchoTID lastTID;
} uEchoController;
	
/****************************************
 * Function
****************************************/

uEchoController *uecho_controller_new(void);
void uecho_controller_delete(uEchoController *cp);
bool uecho_controller_start(uEchoController *cp);
bool uecho_controller_stop(uEchoController *cp);
bool uecho_controller_isrunning(uEchoController *cp);

#define uecho_controller_setlasttid(cp, value) (cp->lastTID = value)
#define uecho_controller_getlasttid(cp, value) (cp->lastTID)
uEchoTID uecho_controller_getnexttid(uEchoController *cp);
  
bool uecho_controller_searchall(uEchoController *cp);
bool uecho_controller_searchobject(uEchoController *cp, byte objCode);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
