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

enum {
  uEchoObjectCodeMin = 0x000000,
  uEchoObjectCodeMax = 0xFFFFFF,
};
  
enum {
  uEchoNodeProfileObject = 0x0EF001,
};
  
/****************************************
 * Data Type
 ****************************************/

typedef struct _uEchoObject {
    bool headFlag;
    struct _uEchoObject *prev;
    struct _uEchoObject *next;

    byte code[3];
} uEchoObject, uEchoObjectList;

/****************************************
 * Function
 ****************************************/

uEchoObject *uecho_object_new();
void uecho_object_delete(uEchoObject *obj);
#define uecho_object_next(obj) (uEchoObject *)uecho_list_next((uEchoList *)obj)
#define uecho_object_remove(obj) uecho_list_remove((uEchoList *)obj)

void uecho_object_setcode(uEchoObject *obj, int val);
int uecho_object_getcode(uEchoObject *obj);
  
bool uecho_object_start(uEchoObject *obj);
bool uecho_object_stop(uEchoObject *obj);
bool uecho_object_isrunning(uEchoObject *obj);

/****************************************
 * Macro
 ****************************************/
    
#if defined(C99)
    
inline void uecho_object_setclassgroupcode(uEchoObject *obj, byte val) {obj->code[0] = val;}
inline byte uecho_object_getclassgroupcode(uEchoObject *obj) {return obj->code[0];}

inline void uecho_object_setclasscode(uEchoObject *obj, byte val) {obj->code[1] = val;}
inline byte uecho_object_getclasscode(uEchoObject *obj) {return obj->code[1];}

inline void uecho_object_setinstancecode(uEchoObject *obj, byte val) {obj->code[2] = val;}
inline byte uecho_object_getinstancecode(uEchoObject *obj) {return obj->code[2];}

#else
    
#define uecho_object_setclassgroupcode(obj, val) (obj->code[0] = val)
#define uecho_object_getclassgroupcode(obj) (obj->code[0])

#define uecho_object_setclasscode(obj, val) (obj->code[1] = val)
#define uecho_object_getclasscode(obj) (obj->code[1])

#define uecho_object_setinstancecode(obj, val) (obj->code[2] = val)
#define uecho_object_getinstancecode(obj) (obj->code[2])
    
#endif
    
#ifdef  __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_NODE_H_ */
