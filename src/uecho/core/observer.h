/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_OBSERVER_H_
#define _UECHO_OBSERVER_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/object.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct {
  UECHO_LIST_MEMBERS
  
  uEchoEsv esv;
  uEchoPropertyCode propCode;
  uEchoPropertyRequestListener listener;
} uEchoObjectPropertyObserver, uEchoObjectPropertyObserverList;

typedef struct _uEchoObjectPropertyObserverManager {
  uEchoObjectPropertyObserverList *observers;
} uEchoObjectPropertyObserverManager;
  
/****************************************
 * Function (Property Observer)
 ****************************************/
  
uEchoObjectPropertyObserver *uecho_object_property_observer_new(void);
void uecho_object_property_observer_delete(uEchoObjectPropertyObserver *obs);
#define uecho_object_property_observer_next(obs) (uEchoObjectPropertyObserver *)uecho_list_next((uEchoList *)obs)
#define uecho_object_property_observer_remove(obs) uecho_list_remove((uEchoList *)obs)
  
void uecho_object_property_observer_setpropetycode(uEchoObjectPropertyObserver *obs, uEchoPropertyCode code);
void uecho_object_property_observer_setesv(uEchoObjectPropertyObserver *obs, uEchoEsv esv);
void uecho_object_property_observer_setlistener(uEchoObjectPropertyObserver *obs, uEchoPropertyRequestListener listener);

uEchoPropertyCode uecho_object_property_observer_getpropetycode(uEchoObjectPropertyObserver *obs);
uEchoEsv uecho_object_property_observer_getesv(uEchoObjectPropertyObserver *obs);
uEchoPropertyRequestListener uecho_object_property_observer_getlistener(uEchoObjectPropertyObserver *obs);

/****************************************
 * Function (Property Observer List)
 ****************************************/
  
uEchoObjectPropertyObserverList *uecho_object_property_observerlist_new();
void uecho_object_property_observerlist_delete(uEchoObjectPropertyObserverList *obsMgr);
bool uecho_object_property_observerlist_setobserver(uEchoObjectPropertyObserverList *obsMgr, uEchoPropertyCode code, uEchoPropertyRequestListener listener);
uEchoObjectPropertyObserver *uecho_object_property_observerlist_getobserver(uEchoObjectPropertyObserverList *obsMgr, uEchoPropertyCode code);
  
#define uecho_object_property_observerlist_clear(obsMgr) uecho_list_clear((uEchoList *)obsMgr, (UECHO_LIST_DESTRUCTORFUNC)uecho_object_property_observer_delete)
#define uecho_object_property_observerlist_size(obsMgr) uecho_list_size((uEchoList *)obsMgr)
#define uecho_object_property_observerlist_gets(obsMgr) (uEchoObjectPropertyObserver *)uecho_list_next((uEchoList *)obsMgr)
#define uecho_object_property_observerlist_add(obsMgr, obs) uecho_list_add((uEchoList *)obsMgr, (uEchoList *)obs)
#define uecho_object_property_observerlist_remove(obs) uecho_list_remove((uEchoList *)obs)

/****************************************
 * Function (Property Observer Manager)
 ****************************************/
  
uEchoObjectPropertyObserverManager *uecho_object_property_observer_manager_new();
void uecho_object_property_observer_manager_delete(uEchoObjectPropertyObserverManager *obsMgr);

bool uecho_object_property_observer_manager_setobserver(uEchoObjectPropertyObserverManager *obsMgr, uEchoEsv esv, uEchoPropertyCode code, uEchoPropertyRequestListener listener);

uEchoObjectPropertyObserver *uecho_object_property_observer_manager_getobservers(uEchoObjectPropertyObserverManager *obsMgr);
uEchoObjectPropertyObserver *uecho_object_property_observer_manager_getobserver(uEchoObjectPropertyObserverManager *obsMgr, uEchoEsv esv, uEchoPropertyCode code);
  
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_OBSERVER_H_ */
