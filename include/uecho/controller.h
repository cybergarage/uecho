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

#include <uecho/const.h>
#include <uecho/node.h>
#include <uecho/typedef.h>

#ifdef __cplusplus
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

typedef void (*uEchoControllerMessageListener)(uEchoController*, uEchoMessage*);
typedef void (*uEchoControllerNodeListener)(uEchoController*, uEchoNode*, uEchoNodeStatus, uEchoMessage*);

/****************************************
 * Function
 ****************************************/

uEchoController* uecho_controller_new(void);
bool uecho_controller_delete(uEchoController* ctrl);

bool uecho_controller_addnode(uEchoController* ctrl, uEchoNode* node);
uEchoNode* uecho_controller_getnodebyaddress(uEchoController* ctrl, const char* addr);
uEchoNode* uecho_controller_getnodes(uEchoController* ctrl);
size_t uecho_controller_getnodecount(uEchoController* ctrl);

void uecho_controller_setmessagelistener(uEchoController* ctrl, uEchoControllerMessageListener listener);
void uecho_controller_setnodelistener(uEchoController* ctrl, uEchoControllerNodeListener listener);

bool uecho_controller_search(uEchoController* ctrl);
bool uecho_controller_searchobject(uEchoController* ctrl, byte obj_code);

bool uecho_controller_announcemessage(uEchoController* ctrl, uEchoMessage* msg);
bool uecho_controller_sendmessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* msg);

void uecho_controller_setpostwaitemilitime(uEchoController* ctrl, clock_t mtime);
clock_t uecho_controller_getpostwaitemilitime(uEchoController* ctrl);
bool uecho_controller_postmessage(uEchoController* ctrl, uEchoNode* node, uEchoMessage* req_msg, uEchoMessage* res_msg);

bool uecho_controller_start(uEchoController* ctrl);
bool uecho_controller_stop(uEchoController* ctrl);
bool uecho_controller_isrunning(uEchoController* ctrl);

void uecho_controller_setuserdata(uEchoController* ctrl, void* data);
void* uecho_controller_getuserdata(uEchoController* ctrl);

uEchoNode* uecho_controller_getlocalnode(uEchoController* ctrl);

/****************************************
 * Function (Node)
 ****************************************/

uEchoController* uecho_node_getcontroller(uEchoNode* node);

#ifdef __cplusplus
}
#endif

#endif /* _UECHO_CONTROLLER_H_ */
