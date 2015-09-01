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

#include <uecho/typedef.h>
#include <uecho/const.h>
#include <uecho/node.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

#define uEchoControllerPostResponseMaxMiliTime 5000
  
/****************************************
* Data Type
****************************************/

#if !defined(_UECHO_CONTROLLER_INTERNAL_H_)
typedef void uEchoController;
#endif
  
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

void uecho_controller_setmessagelistener(uEchoController *ctrl, uEchoControllerMessageListener listener);
uEchoControllerMessageListener uecho_controller_getmessagelistener(uEchoController *ctrl);
bool uecho_controller_hasmessagelistener(uEchoController *ctrl);

bool uecho_controller_searchallobjects(uEchoController *ctrl);
bool uecho_controller_searchobject(uEchoController *ctrl, byte objCode);

bool uecho_controller_announcemessage(uEchoController *ctrl, uEchoMessage *msg);
bool uecho_controller_sendmessage(uEchoController *ctrl, uEchoObject *obj, uEchoMessage *msg);

void uecho_controller_setpostwaitemilitime(uEchoController *ctrl, clock_t mtime);
clock_t uecho_controller_getpostwaitemilitime(uEchoController *ctrl);
bool uecho_controller_postmessage(uEchoController *ctrl, uEchoObject *obj, uEchoMessage *reqMsg, uEchoMessage *resMsg);

bool uecho_controller_start(uEchoController *ctrl);
bool uecho_controller_stop(uEchoController *ctrl);
bool uecho_controller_isrunning(uEchoController *ctrl);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
