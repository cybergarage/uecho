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
  gSharedStdDatabase = NULL;
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
    // Entries are materialized from the const tables when they are looked up.
    gSharedStdDatabase->isStandard = true;
    atexit(uecho_standard_freedatabase);
  }
  return gSharedStdDatabase;
}
