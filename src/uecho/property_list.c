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
* uecho_propertylist_new
****************************************/

uEchoPropertyList *uecho_propertylist_new(void)
{
	uEchoPropertyList *props;

	props = (uEchoPropertyList *)malloc(sizeof(uEchoPropertyList));
  if (!props)
    return NULL;

  uecho_list_header_init((uEchoList *)props);

	return props;
}

/****************************************
* uecho_propertylist_delete
****************************************/

void uecho_propertylist_delete(uEchoPropertyList *props)
{
	uecho_propertylist_clear(props);

	free(props);
}

/****************************************
 * uecho_propertylist_delete
 ****************************************/

uEchoProperty* uecho_propertylist_find(uEchoPropertyList *props, uEchoProperty *prop)
{
  return NULL;
}

/****************************************
 * uecho_propertylist_set
 ****************************************/

bool uecho_propertylist_set(uEchoPropertyList *props, uEchoProperty *prop)
{
  return false;
}
