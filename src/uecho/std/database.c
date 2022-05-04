/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <uecho/std/_database.h>

/****************************************
* uecho_database_new
****************************************/

uEchoDatabase* uecho_database_new(void)
{
  uEchoDatabase* db;

  db = (uEchoDatabase*)malloc(sizeof(uEchoDatabase));

  if (!db)
    return NULL;

  db->mans = uecho_manufacturelist_new();
  db->objs = uecho_objectlist_new();

  if (!db->mans || !db->objs) {
    uecho_database_delete(db);
    return NULL;
  }

  return db;
}

/****************************************
* uecho_database_delete
****************************************/

bool uecho_database_delete(uEchoDatabase* db)
{
  if (!db)
    return false;

  if (db->mans) {
    uecho_manufacturelist_delete(db->mans);
  }
  
  if (db->objs) {
    uecho_objectlist_delete(db->objs);
  }

  free(db);

  return true;
}

/****************************************
* uecho_database_addmanufacture
****************************************/

bool uecho_database_addmanufacture(uEchoDatabase* db, uEchoManufacture *man)
{
  if (!db)
    return false;
  return uecho_manufacturelist_add(db->mans, man);
}

/****************************************
* uecho_database_addobject
****************************************/

bool uecho_database_addobject(uEchoDatabase* db, uEchoObject *obj)
{
  if (!db)
    return false;
  return uecho_objectlist_add(db->objs, obj);
}

/****************************************
* uecho_database_getmanufacture
****************************************/

uEchoManufacture *uecho_database_getmanufacture(uEchoDatabase* db, uEchoManufactureCode code)
{
  if (!db)
    return NULL;
  return uecho_manufacturelist_getbycode(db->mans, code);
}

