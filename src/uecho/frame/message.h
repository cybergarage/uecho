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

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/message_internal.h>

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

uEchoMessageObserver* uecho_message_observer_copy(uEchoMessageObserver* obs);

/****************************************
 * Function (Message Observer List)
 ****************************************/

uEchoMessageObserverList* uecho_message_observerlist_new(void);
void uecho_message_observerlist_delete(uEchoMessageObserverList* obs_list);

#define uecho_message_observerlist_clear(obs_list) uecho_list_clear((uEchoList*)obs_list, (UECHO_LIST_DESTRUCTORFUNC)uecho_message_observer_delete)
#define uecho_message_observerlist_size(obs_list) uecho_list_size((uEchoList*)obs_list)
#define uecho_message_observerlist_gets(obs_list) (uEchoMessageObserver*)uecho_list_next((uEchoList*)obs_list)
#define uecho_message_observerlist_add(obs_list, obs) uecho_list_add((uEchoList*)obs_list, (uEchoList*)obs)

/****************************************
 * Function (Message Observer Manager)
 ****************************************/

uEchoMessageObserverManager* uecho_message_observer_manager_new(void);
void uecho_message_observer_manager_delete(uEchoMessageObserverManager* mgr);

#define uecho_message_observer_manager_getobservers(mgr) uecho_message_observerlist_gets(mgr->observers)

bool uecho_message_observer_manager_addobserver(uEchoMessageObserverManager* mgr, void *obj, uEchoMessageHandler handler);
bool uecho_message_observer_manager_perform(uEchoMessageObserverManager* mgr, uEchoMessage* msg);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_MESSAGE_OBSERVER_H_ */
