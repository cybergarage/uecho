/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONTROLLER_H_
#define _UECHO_CONTROLLER_H_

#include <stdbool.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Data Type
****************************************/

typedef struct _uEchoControlPoint {
	uEchoMutex *mutex;
} uEchoControlPoint;
	
/****************************************
 * Function
****************************************/

uEchoControlPoint *uecho_controller_new();
void uecho_controller_delete(uEchoControlPoint *cp);
bool uecho_controller_start(uEchoControlPoint *cp);
bool uecho_controller_stop(uEchoControlPoint *cp);
bool uecho_controller_isrunning(uEchoControlPoint *cp);
bool uecho_controller_searchall(uEchoControlPoint *cp);
bool uecho_controller_searchobject(uEchoControlPoint *cp);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
