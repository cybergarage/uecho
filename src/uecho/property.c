/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <string.h>

#include <uecho/property.h>

/****************************************
* uecho_property_new
****************************************/

uEchoProperty *uecho_property_new(void) {
	uEchoProperty *prop;

	prop = (uEchoProperty *)malloc(sizeof(uEchoProperty));
    
  if (!prop)
    return NULL;

  uecho_list_node_init((uEchoList *)prop);
  
  prop->count = 0;
  prop->data = NULL;
  
	return prop;
}

/****************************************
* uecho_property_delete
****************************************/

void uecho_property_delete(uEchoProperty *prop) {
  uecho_property_cleardata(prop);
  
	free(prop);
}

/****************************************
 * uecho_property_setdata
 ****************************************/

bool uecho_property_setdata(uEchoProperty *prop, const byte *data, size_t count) {
  uecho_property_cleardata(prop);

  if (count == 0)
    return true;
  
  prop->data = (byte *)malloc(count);
  if (!prop->data)
    return false;
  
  memcpy(prop->data, data, count);
  prop->count = count;

  return true;
}

/****************************************
 * uecho_property_cleardata
 ****************************************/

bool uecho_property_cleardata(uEchoProperty *prop) {
  prop->count = 0;
  
  if (prop->data) {
    free(prop->data);
    prop->data = NULL;
  }
  
  return true;
}
