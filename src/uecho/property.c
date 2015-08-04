/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
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
	uEchoProperty *prop;

	prop = (uEchoProperty *)malloc(sizeof(uEchoProperty));
    
  if (!prop)
    return NULL;

	return prop;
}

/****************************************
* uecho_property_delete
****************************************/

void uecho_property_delete(uEchoProperty *prop)
{
	free(prop);
}

/****************************************
 * uecho_property_setdata
 ****************************************/

void uecho_property_setdata(uEchoProperty *prop, byte *data, size_t count)
{
}

