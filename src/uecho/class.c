/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/class_internal.h>
#include <uecho/misc.h>

/****************************************
* uecho_class_new
****************************************/

uEchoClass *uecho_class_new(void)
{
	uEchoClass *cls;

	cls = (uEchoClass *)malloc(sizeof(uEchoClass));

  if (!cls)
    return NULL;

  uecho_list_node_init((uEchoList *)cls);

  uecho_class_setclassgroupcode(cls, 0);
  uecho_class_setclasscode(cls, 0);

  return cls;
}

/****************************************
* uecho_class_delete
****************************************/

void uecho_class_delete(uEchoClass *cls)
{
  if (!cls)
    return;
  
	uecho_list_remove((uEchoList *)cls);

  free(cls);
}

/****************************************
 * uecho_class_next
 ****************************************/

uEchoClass *uecho_class_next(uEchoClass *cls)
{
  return (uEchoClass *)uecho_list_next((uEchoList *)cls);
}

/****************************************
 * uecho_class_setcode
 ****************************************/

void uecho_class_setcode(uEchoClass *cls, uEchoClassCode val)
{
  if (!cls)
    return;

  cls->code[0] = (val & 0xFF00) >> 8;
  cls->code[1] = (val & 0x00FF);
}

/****************************************
 * uecho_class_getcode
 ****************************************/

uEchoClassCode uecho_class_getcode(uEchoClass *cls)
{
  int code;
  
  if (!cls)
    return 0;
  
  code = 0;
  code |= (cls->code[0] << 8) & 0xFF00;
  code |= (cls->code[1]     ) & 0x00FF;
  return code;
}

/****************************************
 * uecho_class_getcode
 ****************************************/

bool uecho_class_iscode(uEchoClass *cls, uEchoClassCode code)
{
  if (!cls)
    return false;

  return (uecho_class_getcode(cls) == code) ? true : false;
}

/****************************************
 * uecho_class_setclassgroupcode
 ****************************************/

void uecho_class_setclassgroupcode(uEchoClass *cls, byte val)
{
  if (!cls)
    return;
  
  cls->code[0] = val;
}

/****************************************
 * uecho_class_getclassgroupcode
 ****************************************/

byte uecho_class_getclassgroupcode(uEchoClass *cls)
{
  if (!cls)
    return 0;
  
  return cls->code[0];
}

/****************************************
 * uecho_class_setclasscode
 ****************************************/

void uecho_class_setclasscode(uEchoClass *cls, byte val)
{
  if (!cls)
    return;

  cls->code[1] = val;
}

/****************************************
 * uecho_class_getclasscode
 ****************************************/

byte uecho_class_getclasscode(uEchoClass *cls)
{
  if (!cls)
    return 0;

  return cls->code[1];
}

/****************************************
 * uecho_class_isdevice
 ****************************************/

bool uecho_class_isdevice(uEchoClass *cls)
{
  if (!cls)
    return false;
  
  return uecho_isdeviceclassgroupcode(cls->code[0]);
}

/****************************************
 * uecho_class_isprofile
 ****************************************/

bool uecho_class_isprofile(uEchoClass *cls)
{
  if (!cls)
    return false;
  
  return uecho_isprofileclassgroupcode(cls->code[0]);
}
