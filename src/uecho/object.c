/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/object.h>

/****************************************
* uecho_object_new
****************************************/

uEchoObject *uecho_object_new() {
	uEchoObject *obj;

	obj = (uEchoObject *)malloc(sizeof(uEchoObject));
    
    if (!obj)
        return NULL;

    uecho_list_node_init((uEchoList *)obj);

    uecho_object_setclassgroupcode(obj, 0);
    uecho_object_setclasscode(obj, 0);
    uecho_object_setinstancecode(obj, 0);
    
	return obj;
}

/****************************************
* uecho_object_delete
****************************************/

void uecho_object_delete(uEchoObject *obj)
{
	uecho_list_remove((uEchoList *)obj);
    
	free(obj);
}

/****************************************
 * uecho_object_setcode
 ****************************************/

void uecho_object_setcode(uEchoObject *obj, int val) {
  obj->code[0] = (val & 0xFF0000) >> 16;
  obj->code[1] = (val & 0x00FF00) >>  8;
  obj->code[2] = (val & 0x0000FF);
}

/****************************************
 * uecho_object_getcode
 ****************************************/

int uecho_object_getcode(uEchoObject *obj) {
  int code = 0;
  code |= (obj->code[0] << 16) & 0xFF0000;
  code |= (obj->code[1] <<  8) & 0x00FF00;
  code |= (obj->code[2]      ) & 0x0000FF;
  return code;
}

