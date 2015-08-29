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
  void (*msgListener)(struct _uEchoController *, uEchoMessage *); /* uEchoControllerMessageListener */
} uEchoController;

typedef void (*uEchoControllerMessageListener)(uEchoController *, uEchoMessage *);

/****************************************
 * Function
****************************************/

uEchoController *uecho_controller_new(void);
void uecho_controller_delete(uEchoController *ctrl);

bool uecho_controller_addnode(uEchoController *ctrl, uEchoNode *node);
uEchoNode *uecho_controller_getnodebyaddress(uEchoController *ctrl, const char *addr);
uEchoNode *uecho_controller_getnodes(uEchoController *ctrl);
size_t uecho_controller_getnodecount(uEchoController *ctrl);

uEchoObject *uecho_controller_getobjectbycode(uEchoController *ctrl, uEchoObjectCode code);
  
void uecho_controller_setlasttid(uEchoController *ctrl, uEchoTID tid);
uEchoTID uecho_controller_getlasttid(uEchoController *ctrl);
uEchoTID uecho_controller_getnexttid(uEchoController *ctrl);

void uecho_controller_setmessagelistener(uEchoController *ctrl, uEchoControllerMessageListener listener);
uEchoControllerMessageListener uecho_controller_getmessagelistener(uEchoController *ctrl);
bool uecho_controller_hasmessagelistener(uEchoController *ctrl);

bool uecho_controller_searchallobjects(uEchoController *ctrl);
bool uecho_controller_searchobject(uEchoController *ctrl, byte objCode);

bool uecho_controller_announcemessage(uEchoController *ctrl, uEchoMessage *msg);
bool uecho_controller_sendmessage(uEchoController *ctrl, uEchoObject *obj, uEchoMessage *msg);

bool uecho_controller_start(uEchoController *ctrl);
bool uecho_controller_stop(uEchoController *ctrl);
bool uecho_controller_isrunning(uEchoController *ctrl);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
