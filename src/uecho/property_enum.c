/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/_node.h>
#include <uecho/_property.h>
#include <uecho/profile.h>

#include <uecho/misc.h>

/****************************************
 * uecho_property_enum_new
 ****************************************/

uEchoPropertyEnum* uecho_property_enum_new(void)
{
  uEchoPropertyEnum* prop;

  prop = (uEchoPropertyEnum*)malloc(sizeof(uEchoPropertyEnum));

  if (!prop)
    return NULL;

  uecho_list_node_init((uEchoList*)prop);

  prop->code = 0x00;
  prop->name = uecho_string_new();
  prop->desc = uecho_string_new();

  if (!prop->name || !prop->desc) {
    uecho_property_enum_delete(prop);
    return NULL;
  }

  return prop;
}

/****************************************
 * uecho_property_enum_delete
 ****************************************/

bool uecho_property_enum_delete(uEchoPropertyEnum* prop)
{
  if (!prop)
    return false;

  if (prop->name) {
    uecho_string_delete(prop->name);
  }

  if (prop->desc) {
    uecho_string_delete(prop->desc);
  }

  uecho_property_enum_remove(prop);

  free(prop);

  return true;
}

/****************************************
 * uecho_property_enum_next
 ****************************************/

uEchoPropertyEnum* uecho_property_enum_next(uEchoPropertyEnum* prop)
{
  return (uEchoPropertyEnum*)uecho_list_next((uEchoList*)prop);
}

/****************************************
 * uecho_property_enum_remove
 ****************************************/

void uecho_property_enum_remove(uEchoPropertyEnum* prop)
{
  uecho_list_remove((uEchoList*)prop);
}

/****************************************
 * uecho_property_enum_setcode
 ****************************************/

void uecho_property_enum_setcode(uEchoPropertyEnum* prop, int val)
{
  if (!prop)
    return;
  prop->code = val;
}

/****************************************
 * uecho_property_enum_getcode
 ****************************************/

int uecho_property_enum_getcode(uEchoPropertyEnum* prop)
{
  if (!prop)
    return 0;
  return prop->code;
}

/****************************************
 * uecho_property_enum_setname
 ****************************************/

void uecho_property_enum_setname(uEchoPropertyEnum* prop, const char* name)
{
  if (!prop)
    return;
  uecho_string_setvalue(prop->name, name);
}

/****************************************
 * uecho_property_enum_getname
 ****************************************/

const char* uecho_property_enum_getname(uEchoPropertyEnum* prop)
{
  if (!prop)
    return NULL;
  return uecho_string_getvalue(prop->name);
}

/****************************************
 * uecho_property_enum_setdescription
 ****************************************/

void uecho_property_enum_setdescription(uEchoPropertyEnum* prop, const char* desc)
{
  if (!prop)
    return;
  uecho_string_setvalue(prop->desc, desc);
}

/****************************************
 * uecho_property_enum_getdescription
 ****************************************/

const char* uecho_property_enum_getdescription(uEchoPropertyEnum* prop)
{
  if (!prop)
    return NULL;
  return uecho_string_getvalue(prop->desc);
}
