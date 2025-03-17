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

static uEchoDatabase* gSharedStdDatabase = NULL;

void uecho_standard_freedatabase(void)
{
  if (!gSharedStdDatabase)
    return;
  uecho_database_delete(gSharedStdDatabase);
}

/****************************************
 * uecho_standard_getdatabase
 ****************************************/

uEchoDatabase* uecho_standard_getdatabase(void)
{
  if (!gSharedStdDatabase) {
    gSharedStdDatabase = uecho_database_new();
    if (!gSharedStdDatabase)
      return NULL;
    uecho_database_addstandardmanufactures(gSharedStdDatabase);
    uecho_database_addstandardobjects(gSharedStdDatabase);
    atexit(uecho_standard_freedatabase);
  }
  return gSharedStdDatabase;
}
