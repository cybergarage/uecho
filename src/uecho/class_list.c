/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/class.h>

/****************************************
* uecho_classlist_new
****************************************/

uEchoClassList *uecho_classlist_new(void)
{
	uEchoClassList *clsses;

	clsses = (uEchoClassList *)malloc(sizeof(uEchoClassList));
  if (!clsses)
    return NULL;

  uecho_list_header_init((uEchoList *)clsses);

	return clsses;
}

/****************************************
* uecho_classlist_delete
****************************************/

void uecho_classlist_delete(uEchoClassList *clsses)
{
  if (!clsses)
    return;
  
	uecho_classlist_clear(clsses);

	free(clsses);
}

/****************************************
 * uecho_classlist_getbycode
 ****************************************/

uEchoClass *uecho_classlist_getbycode(uEchoClassList *clsses, uEchoClassCode code)
{
  uEchoClass *obj;
  
  if (!clsses)
    return NULL;
  
  for (obj = uecho_classlist_gets(clsses); obj; obj = uecho_class_next(obj)) {
    if (uecho_class_getcode(obj) == code)
      return obj;
  }
  
  return NULL;
}

/****************************************
 * uecho_classlist_set
 ****************************************/

bool uecho_classlist_set(uEchoClassList *clsses, uEchoClassCode code)
{
  uEchoClass *obj;
  
  if (!clsses)
    return false;
  
  obj = uecho_classlist_getbycode(clsses, code);
  if (obj)
    return true;
  
  obj = uecho_class_new();
  if (!obj)
    return false;
  
  uecho_class_setcode(obj, code);
  uecho_classlist_add(clsses, obj);
  
  return true;
}
