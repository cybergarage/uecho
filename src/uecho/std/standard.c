/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <stdlib.h>

#include <uecho/std/_database.h>

static uEchoDatabase* _uecho_std_database = NULL;

void uecho_standard_freedatabase(void)
{
  if (!_uecho_std_database)
    return;
  uecho_database_delete(_uecho_std_database);
}

/****************************************
* uecho_standard_getdatabase
****************************************/

uEchoDatabase* uecho_standard_getdatabase(void)
{
  if (!_uecho_std_database) {
    _uecho_std_database = uecho_database_new();
    if (!_uecho_std_database)
      return NULL;
    uecho_database_addstandardmanufactures(_uecho_std_database);
    uecho_database_addstandardobjects(_uecho_std_database);
    atexit(uecho_standard_freedatabase);
  }
  return _uecho_std_database;
}
