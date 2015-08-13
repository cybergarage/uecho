/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/object.h>

/****************************************
* uecho_objectlist_new
****************************************/

uEchoObjectList *uecho_objectlist_new(void)
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

/****************************************
 * uecho_objectlist_getbycode
 ****************************************/

uEchoObject *uecho_objectlist_getbycode(uEchoObjectList *objs, uEchoObjectCode code)
{
  uEchoObject *obj;
  
  for (obj = uecho_objectlist_gets(objs); obj; obj = uecho_object_next(obj))
{
    if (uecho_object_getcode(obj) == code)
      return obj;
  }
  
  return NULL;
}

/****************************************
 * uecho_objectlist_set
 ****************************************/

bool uecho_objectlist_set(uEchoObjectList *objs, uEchoObjectCode code)
{
  uEchoObject *obj;
  
  obj = uecho_objectlist_getbycode(objs, code);
  if (obj)
    return true;
  
  obj = uecho_object_new();
  if (!obj)
    return false;
  
  uecho_object_setcode(obj, code);
  uecho_objectlist_add(objs, obj);
  
  return true;
}
