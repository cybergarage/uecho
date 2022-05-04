/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_PROPERTY_OBSERVER_H_
#define _UECHO_PROPERTY_OBSERVER_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct {
  UECHO_LIST_STRUCT_MEMBERS

  uEchoEsv esv;
  uEchoPropertyCode prop_code;
  uEchoPropertyRequestHandler handler;
} uEchoObjectPropertyObserver, uEchoObjectPropertyObserverList;

typedef struct {
  uEchoObjectPropertyObserverList* observers;
} uEchoObjectPropertyObserverManager;

/****************************************
 * Function (Property Observer)
 ****************************************/

uEchoObjectPropertyObserver* uecho_object_property_observer_new(void);
void uecho_object_property_observer_delete(uEchoObjectPropertyObserver* obs);
#define uecho_object_property_observer_next(obs) (uEchoObjectPropertyObserver*)uecho_list_next((uEchoList*)obs)
#define uecho_object_property_observer_remove(obs) uecho_list_remove((uEchoList*)obs)

void uecho_object_property_observer_setpropetycode(uEchoObjectPropertyObserver* obs, uEchoPropertyCode code);
void uecho_object_property_observer_setesv(uEchoObjectPropertyObserver* obs, uEchoEsv esv);
void uecho_object_property_observer_sethandler(uEchoObjectPropertyObserver* obs, uEchoPropertyRequestHandler handler);

uEchoPropertyCode uecho_object_property_observer_getpropetycode(uEchoObjectPropertyObserver* obs);
uEchoEsv uecho_object_property_observer_getesv(uEchoObjectPropertyObserver* obs);
uEchoPropertyRequestHandler uecho_object_property_observer_handler(uEchoObjectPropertyObserver* obs);

/****************************************
 * Function (Property Observer List)
 ****************************************/

uEchoObjectPropertyObserverList* uecho_object_property_observerlist_new(void);
void uecho_object_property_observerlist_delete(uEchoObjectPropertyObserverList* obs_list);
bool uecho_object_property_observerlist_setobserver(uEchoObjectPropertyObserverList* obs_list, uEchoPropertyCode code, uEchoPropertyRequestHandler handler);
uEchoObjectPropertyObserver* uecho_object_property_observerlist_getobserver(uEchoObjectPropertyObserverList* obs_list, uEchoPropertyCode code);

#define uecho_object_property_observerlist_clear(obs_list) uecho_list_clear((uEchoList*)obs_list, (UECHO_LIST_DESTRUCTORFUNC)uecho_object_property_observer_delete)
#define uecho_object_property_observerlist_size(obs_list) uecho_list_size((uEchoList*)obs_list)
#define uecho_object_property_observerlist_gets(obs_list) (uEchoObjectPropertyObserver*)uecho_list_next((uEchoList*)obs_list)
#define uecho_object_property_observerlist_add(obs_list, obs) uecho_list_add((uEchoList*)obs_list, (uEchoList*)obs)

/****************************************
 * Function (Property Observer Manager)
 ****************************************/

uEchoObjectPropertyObserverManager* uecho_object_property_observer_manager_new(void);
void uecho_object_property_observer_manager_delete(uEchoObjectPropertyObserverManager* mgr);

bool uecho_object_property_observer_manager_setobserver(uEchoObjectPropertyObserverManager* mgr, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestHandler handler);

uEchoObjectPropertyObserver* uecho_object_property_observer_manager_getobservers(uEchoObjectPropertyObserverManager* mgr);
uEchoObjectPropertyObserver* uecho_object_property_observer_manager_getobserver(uEchoObjectPropertyObserverManager* mgr, uEchoEsv esv, uEchoPropertyCode code);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_PROPERTY_OBSERVER_H_ */
