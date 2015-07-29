/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/property.h>

/****************************************
* uecho_property_new
****************************************/

uEchoProperty *uecho_property_new()
{
	uEchoProperty *property;

	property = (uEchoProperty *)malloc(sizeof(uEchoProperty));
    
    if (!property)
        return NULL;

    uecho_list_node_init((uEchoList *)property);
	
	return property;
}

/****************************************
* uecho_property_delete
****************************************/

void uecho_property_delete(uEchoProperty *property)
{
	uecho_list_remove((uEchoList *)property);
    
	free(property);
}

/****************************************
* uecho_property_clear
****************************************/

void uecho_property_clear(uEchoProperty *property)
{
}
