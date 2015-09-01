/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONTROLLER_INTERNAL_H_
#define _UECHO_CONTROLLER_INTERNAL_H_

#include <uecho/typedef.h>
#include <uecho/const.h>
#include <uecho/util/mutex.h>
#include <uecho/node_internal.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoControllerParamNone             = 0x00,
  uEchoControllerParamDisableUdpServer = 0x01,
};
  
/****************************************
* Data Type
****************************************/

typedef int uEchoControllerParam;
  
typedef struct _uEchoController {
	uEchoMutex *mutex;
  uEchoNode *node;
  uEchoTID lastTID;
  uEchoNodeList *nodes;
  void (*msgListener)(struct _uEchoController *, uEchoMessage *); /* uEchoControllerMessageListener */
  uEchoControllerParam param;
  
  clock_t postResWaitMiliTime;
  uEchoMessage *postReqMsg;
  uEchoMessage *postResMsg;
} uEchoController;

/****************************************
 * Header
 ****************************************/

#include <uecho/controller.h>

/****************************************
 * Function
****************************************/

void uecho_controller_enableparameter(uEchoController *ctrl, uEchoControllerParam param);
void uecho_controller_disableparameter(uEchoController *ctrl, uEchoControllerParam param);
bool uecho_controller_isparameterenabled(uEchoController *ctrl, uEchoControllerParam param);
  
void uecho_controller_setlasttid(uEchoController *ctrl, uEchoTID tid);
uEchoTID uecho_controller_getlasttid(uEchoController *ctrl);
uEchoTID uecho_controller_getnexttid(uEchoController *ctrl);  

void uecho_controller_setpostrequestmessage(uEchoController *ctrl, uEchoMessage *msg);
uEchoMessage *uecho_controller_getpostrequestmessage(uEchoController *ctrl);
bool uecho_controller_haspostrequestmessage(uEchoController *ctrl);

void uecho_controller_setpostresponsemessage(uEchoController *ctrl, uEchoMessage *msg);
uEchoMessage *uecho_controller_getpostresponsemessage(uEchoController *ctrl);
bool uecho_controller_haspostresponsemessage(uEchoController *ctrl);
 
bool uecho_controller_ispostresponsemessage(uEchoController *ctrl, uEchoMessage *msg);
bool uecho_controller_ispostresponsereceived(uEchoController *ctrl);
bool uecho_controller_ispostresponsewaiting(uEchoController *ctrl);

void uecho_controller_servermessagelistener(uEchoServer *server, uEchoMessage *msg);

#ifdef  __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_INTERNAL_H_ */
