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

static uEchoDatabase* g_shared_std_database = NULL;

void uecho_standard_freedatabase(void)
{
  if (!g_shared_std_database)
    return;
  uecho_database_delete(g_shared_std_database);
}

/****************************************
 * uecho_standard_getdatabase
 ****************************************/

uEchoDatabase* uecho_standard_getdatabase(void)
{
  if (!g_shared_std_database) {
    g_shared_std_database = uecho_database_new();
    if (!g_shared_std_database)
      return NULL;
    uecho_database_addstandardmanufactures(g_shared_std_database);
    uecho_database_addstandardobjects(g_shared_std_database);
    atexit(uecho_standard_freedatabase);
  }
  return g_shared_std_database;
}
