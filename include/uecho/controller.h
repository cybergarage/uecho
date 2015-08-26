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
#include <uecho/node.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
* Data Type
****************************************/

typedef struct _uEchoController {
	uEchoMutex *mutex;
  uEchoNode *node;
  uEchoTID lastTID;
  uEchoNodeList *nodes;
} uEchoController;
	
/****************************************
 * Function
****************************************/

uEchoController *uecho_controller_new(void);
void uecho_controller_delete(uEchoController *cp);

bool uecho_controller_addnode(uEchoController *cp, uEchoNode *node);
uEchoNode *uecho_controller_getnodebyaddress(uEchoController *cp, const char *addr);
uEchoNode *uecho_controller_getnodes(uEchoController *cp);

void uecho_controller_setlasttid(uEchoController *cp, uEchoTID tid);
uEchoTID uecho_controller_getlasttid(uEchoController *cp);
uEchoTID uecho_controller_getnexttid(uEchoController *cp);

bool uecho_controller_start(uEchoController *cp);
bool uecho_controller_stop(uEchoController *cp);
bool uecho_controller_isrunning(uEchoController *cp);
  
bool uecho_controller_searchall(uEchoController *cp);
bool uecho_controller_searchobject(uEchoController *cp, byte objCode);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
