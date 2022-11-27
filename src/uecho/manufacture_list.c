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
 * uecho_manufacturelist_new
 ****************************************/

uEchoManufactureList* uecho_manufacturelist_new(void)
{
  uEchoManufactureList* mans;

  mans = (uEchoManufactureList*)malloc(sizeof(uEchoManufactureList));
  if (!mans)
    return NULL;

  uecho_list_header_init((uEchoList*)mans);

  return mans;
}

/****************************************
 * uecho_manufacturelist_delete
 ****************************************/

void uecho_manufacturelist_delete(uEchoManufactureList* mans)
{
  if (!mans)
    return;

  uecho_manufacturelist_clear(mans);

  free(mans);
}

/****************************************
 * uecho_manufacturelist_getbycode
 ****************************************/

uEchoManufacture* uecho_manufacturelist_getbycode(uEchoManufactureList* mans, uEchoManufactureCode code)
{
  uEchoManufacture* man;

  if (!mans)
    return NULL;

  for (man = uecho_manufacturelist_gets(mans); man; man = uecho_manufacture_next(man)) {
    if (uecho_manufacture_getcode(man) == code)
      return man;
  }

  return NULL;
}
