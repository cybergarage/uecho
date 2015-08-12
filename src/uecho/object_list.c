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
* uecho_objectlist_new
****************************************/

uEchoObjectList *uecho_objectlist_new()
{
	uEchoObjectList *objs;

	objs = (uEchoObjectList *)malloc(sizeof(uEchoObjectList));
  if (!objs)
    return NULL;

  uecho_list_header_init((uEchoList *)objs);

	return objs;
}

/****************************************
* uecho_objectlist_delete
****************************************/

void uecho_objectlist_delete(uEchoObjectList *objs)
{
	uecho_objectlist_clear(objs);

	free(objs);
}
