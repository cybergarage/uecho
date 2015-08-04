/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_OBJECT_H_
#define _UECHO_OBJECT_H_

#include <uecho/typedef.h>
#include <uecho/util/list.h>
#include <uecho/util/mutex.h>

#ifdef  __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoObject {
    bool headFlag;
    struct _uEchoObject *prev;
    struct _uEchoObject *next;

    byte X[3];
} uEchoObject, uEchoObjectList;

/****************************************
 * Function
 ****************************************/

uEchoObject *uecho_object_new();
void uecho_object_delete(uEchoObject *obj);
#define uecho_object_next(obj) (uEchoObject *)uecho_list_next((uEchoList *)obj)
#define uecho_object_remove(obj) uecho_list_remove((uEchoList *)obj)

bool uecho_object_start(uEchoObject *obj);
bool uecho_object_stop(uEchoObject *obj);
bool uecho_object_isrunning(uEchoObject *obj);

/****************************************
 * Macro
 ****************************************/
    
#if defined(C99)
    
inline void uecho_object_setclassgroupcode(uEchoObject *obj, byte val) {obj->X[0] = val;}
inline byte uecho_object_getclassgroupcode(uEchoObject *obj) {return obj->X[0];}

inline void uecho_object_setclasscode(uEchoObject *obj, byte val) {obj->X[1] = val;}
inline byte uecho_object_getclasscode(uEchoObject *obj) {return obj->X[1];}

inline void uecho_object_setinstancecode(uEchoObject *obj, byte val) {obj->X[2] = val;}
inline byte uecho_object_getinstancecode(uEchoObject *obj) {return obj->X[2];}

#else
    
#define uecho_object_setclassgroupcode(obj, val) (obj->X[0] = val)
#define uecho_object_getclassgroupcode(obj) (obj->X[0])

#define uecho_object_setclasscode(obj, val) (obj->X[1] = val)
#define uecho_object_getclasscode(obj) (obj->X[1])

#define uecho_object_setinstancecode(obj, val) (obj->X[2] = val)
#define uecho_object_getinstancecode(obj) (obj->X[2])
    
#endif
    
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
