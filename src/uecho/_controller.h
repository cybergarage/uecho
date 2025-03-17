/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_CONTROLLER_INTERNAL_H_
#define _UECHO_CONTROLLER_INTERNAL_H_

#include <uecho/_node.h>
#include <uecho/const.h>
#include <uecho/typedef.h>
#include <uecho/util/cond.h>
#include <uecho/util/mutex.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Constant
 ****************************************/

enum {
  uEchoControllerOptionDisableUdpServer = uEchoServerOptionDisableUdpServer,
};

/****************************************
 * Data Type
 ****************************************/

typedef struct UEchoController {
  uEchoMutex* mutex;
  uEchoCond* cond;
  uEchoNode* node;
  uEchoNodeList* nodes;
  void (*msgListener)(struct UEchoController*, uEchoMessage*); /* uEchoControllerMessageListener */
  void (*nodeListener)(struct UEchoController*, uEchoNode*, uEchoNodeStatus, uEchoMessage*); /* uEchoControllerNodeListener */
  uEchoOption option;
  void* userData;

  clock_t postResWaitClockTime;
  uEchoMessage* postReqMsg;
  uEchoMessage* postResMsg;
} uEchoController;

/****************************************
 * Header
 ****************************************/

#include <uecho/controller.h>

/****************************************
 * Function
 ****************************************/

bool uecho_controller_searchwithesv(uEchoController* ctrl, uEchoEsv esv);
bool uecho_controller_searchobjectwithesv(uEchoController* ctrl, byte objCode, uEchoEsv esv);

uEchoObject* uecho_controller_getobjectbycode(uEchoController* ctrl, uEchoObjectCode code);
uEchoObject* uecho_controller_getobjectbycodewithwait(uEchoController* ctrl, uEchoObjectCode code, clock_t waitMiliTime);

void uecho_controller_enableoption(uEchoController* ctrl, uEchoOption param);
void uecho_controller_disableoption(uEchoController* ctrl, uEchoOption param);
bool uecho_controller_isoptionenabled(uEchoController* ctrl, uEchoOption param);
void uecho_controller_disableudpserver(uEchoController* ctrl);

#define uecho_controller_enableudpserver(ctrl) uecho_controller_disableoption(ctrl, uEchoControllerOptionDisableUdpServer)
#define uecho_controller_isudpserverenabled(ctrl) (!uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer))

uEchoControllerMessageListener uecho_controller_getmessagelistener(uEchoController* ctrl);
bool uecho_controller_hasmessagelistener(uEchoController* ctrl);

void uecho_controller_setpostrequestmessage(uEchoController* ctrl, uEchoMessage* msg);
uEchoMessage* uecho_controller_getpostrequestmessage(uEchoController* ctrl);
bool uecho_controller_haspostrequestmessage(uEchoController* ctrl);

void uecho_controller_setpostresponsemessage(uEchoController* ctrl, uEchoMessage* msg);
uEchoMessage* uecho_controller_getpostresponsemessage(uEchoController* ctrl);
bool uecho_controller_haspostresponsemessage(uEchoController* ctrl);

bool uecho_controller_ispostresponsemessage(uEchoController* ctrl, uEchoMessage* msg);
bool uecho_controller_ispostresponsereceived(uEchoController* ctrl);
bool uecho_controller_ispostresponsewaiting(uEchoController* ctrl);

void uecho_controller_servermessagelistener(uEchoController* ctrl, uEchoMessage* msg);

/****************************************
 * Function (Node)
 ****************************************/

void uecho_node_setcontroller(uEchoNode* node, uEchoController* ctrl);

#ifdef __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_INTERNAL_H_ */
