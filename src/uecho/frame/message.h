/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_MESSAGE_OBSERVER_H_
#define _UECHO_MESSAGE_OBSERVER_H_

#include <uecho/object.h>
#include <uecho/typedef.h>
#include <uecho/util/list.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef void (*uEchoMessageHandler)(void*, uEchoMessage*);

typedef struct {
  UECHO_LIST_STRUCT_MEMBERS
  void *object;
  uEchoMessageHandler handler;
} uEchoMessageObserver, uEchoMessageObserverList;

typedef struct {
  uEchoMessageObserverList* observers;
} uEchoMessageObserverManager;

/****************************************
 * Function (Message Observer)
 ****************************************/

uEchoMessageObserver* uecho_message_observer_new(void);
void uecho_message_observer_delete(uEchoMessageObserver* obs);
#define uecho_message_observer_next(obs) (uEchoMessageObserver*)uecho_list_next((uEchoList*)obs)
#define uecho_message_observer_remove(obs) uecho_list_remove((uEchoList*)obs)

void uecho_message_observer_setobject(uEchoMessageObserver* obs, void*obj);
void uecho_message_observer_sethandler(uEchoMessageObserver* obs, uEchoMessageHandler handler);

void *uecho_message_observer_getobjcet(uEchoMessageObserver* obs);
uEchoMessageHandler uecho_message_observer_gethandler(uEchoMessageObserver* obs);

/****************************************
 * Function (Message Observer List)
 ****************************************/

uEchoMessageObserverList* uecho_message_observerlist_new(void);
void uecho_message_observerlist_delete(uEchoMessageObserverList* obs_mgr);
bool uecho_message_observerlist_setobserver(uEchoMessageObserverList* obs_mgr, uEchoPropertyCode code, uEchoMessageHandler handler);
uEchoMessageObserver* uecho_message_observerlist_getobserver(uEchoMessageObserverList* obs_mgr, uEchoPropertyCode code);

#define uecho_message_observerlist_clear(obsMgr) uecho_list_clear((uEchoList*)obsMgr, (UECHO_LIST_DESTRUCTORFUNC)uecho_message_observer_delete)
#define uecho_message_observerlist_size(obsMgr) uecho_list_size((uEchoList*)obsMgr)
#define uecho_message_observerlist_gets(obsMgr) (uEchoMessageObserver*)uecho_list_next((uEchoList*)obsMgr)
#define uecho_message_observerlist_add(obsMgr, obs) uecho_list_add((uEchoList*)obsMgr, (uEchoList*)obs)
#define uecho_message_observerlist_remove(obs) uecho_list_remove((uEchoList*)obs)

/****************************************
 * Function (Message Observer Manager)
 ****************************************/

uEchoMessageObserverManager* uecho_message_observer_manager_new(void);
void uecho_message_observer_manager_delete(uEchoMessageObserverManager* obs_mgr);

bool uecho_message_observer_manager_addobserver(uEchoMessageObserverManager* obs_mgr, void *obj, uEchoMessageHandler handler);
uEchoMessageObserver* uecho_message_observer_manager_getobservers(uEchoMessageObserverManager* obs_mgr);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MESSAGE_OBSERVER_H_ */
