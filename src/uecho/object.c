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

uEchoObject *uecho_object_new()
{
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
