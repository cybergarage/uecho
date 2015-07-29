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
	uEchoObject *object;

	object = (uEchoObject *)malloc(sizeof(uEchoObject));
    
    if (!object)
        return NULL;

    uecho_list_node_init((uEchoList *)object);
	
	return object;
}

/****************************************
* uecho_object_delete
****************************************/

void uecho_object_delete(uEchoObject *object)
{
	uecho_list_remove((uEchoList *)object);
    
	free(object);
}

/****************************************
* uecho_object_clear
****************************************/

void uecho_object_clear(uEchoObject *object)
{
}
