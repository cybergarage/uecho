/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/_manufacture.h>

/****************************************
 * uecho_manufacture_new
 ****************************************/

uEchoManufacture* uecho_manufacture_new(void)
{
  uEchoManufacture* man;

  man = (uEchoManufacture*)malloc(sizeof(uEchoManufacture));

  if (!man)
    return NULL;

  uecho_list_node_init((uEchoList*)man);

  man->name = uecho_string_new();
  uecho_manufacture_setcode(man, 0);

  return man;
}

/****************************************
 * uecho_manufacture_delete
 ****************************************/

bool uecho_manufacture_delete(uEchoManufacture* man)
{
  if (!man)
    return false;

  uecho_list_remove((uEchoList*)man);
  uecho_string_delete(man->name);

  free(man);

  return true;
}

/****************************************
 * uecho_manufacture_next
 ****************************************/

uEchoManufacture* uecho_manufacture_next(uEchoManufacture* man)
{
  return (uEchoManufacture*)uecho_list_next((uEchoList*)man);
}

/****************************************
 * uecho_manufacture_setcode
 ****************************************/

void uecho_manufacture_setcode(uEchoManufacture* man, uEchoManufactureCode val)
{
  if (!man)
    return;
  man->code = val;
}

/****************************************
 * uecho_manufacture_getcode
 ****************************************/

uEchoManufactureCode uecho_manufacture_getcode(uEchoManufacture* man)
{
  if (!man)
    return 0;
  return man->code;
}

/****************************************
 * uecho_manufacture_iscode
 ****************************************/

bool uecho_manufacture_iscode(uEchoManufacture* man, uEchoManufactureCode code)
{
  if (!man)
    return false;
  return (man->code == code) ? true : false;
}

/****************************************
 * uecho_manufacture_setname
 ****************************************/

void uecho_manufacture_setname(uEchoManufacture* man, const char* val)
{
  if (!man)
    return;
  uecho_string_setvalue(man->name, val);
}

/****************************************
 * uecho_manufacture_getname
 ****************************************/

const char* uecho_manufacture_getname(uEchoManufacture* man)
{
  if (!man)
    return NULL;
  return uecho_string_getvalue(man->name);
}
