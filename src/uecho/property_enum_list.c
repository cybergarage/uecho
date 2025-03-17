/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_property.h>

/****************************************
 * uecho_property_enumlist_new
 ****************************************/

uEchoPropertyEnumList* uecho_property_enumlist_new(void)
{
  uEchoPropertyEnumList* props;

  props = (uEchoPropertyEnumList*)malloc(sizeof(uEchoPropertyEnumList));
  if (!props)
    return NULL;

  uecho_list_header_init((uEchoList*)props);

  return props;
}

/****************************************
 * uecho_property_enumlist_delete
 ****************************************/

void uecho_property_enumlist_delete(uEchoPropertyEnumList* props)
{
  if (!props)
    return;

  uecho_property_enumlist_clear(props);

  free(props);
}

/****************************************
 * uecho_property_enumlist_findbycode
 ****************************************/

uEchoPropertyEnum* uecho_property_enumlist_findbycode(uEchoPropertyEnumList* props, int code)
{
  uEchoPropertyEnum* prop;

  if (!props)
    return NULL;

  for (prop = uecho_property_enumlist_gets(props); prop; prop = uecho_property_enum_next(prop)) {
    if (uecho_property_enum_getcode(prop) == code)
      return prop;
  }

  return NULL;
}

/****************************************
 * uecho_property_enumlist_add
 ****************************************/

bool uecho_property_enumlist_add(uEchoPropertyEnumList* props, uEchoPropertyEnum* prop)
{
  uEchoPropertyEnum* addedProp;

  if (!props || !prop)
    return false;

  addedProp = uecho_property_enumlist_findbycode(props, uecho_property_enum_getcode(prop));
  if (addedProp) {
    uecho_property_enum_remove(addedProp);
    uecho_property_enum_delete(addedProp);
  }

  return uecho_list_add((uEchoList*)props, (uEchoList*)prop);
}
