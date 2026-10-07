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
  db->isStandard = false;
  db->mutex = uecho_mutex_new();

  if (!db->mans || !db->objs || !db->mutex) {
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

  if (db->mutex) {
    uecho_mutex_delete(db->mutex);
  }

  free(db);

  return true;
}

/****************************************
 * uecho_database_addmanufacture
 ****************************************/

bool uecho_database_addmanufacture(uEchoDatabase* db, uEchoManufacture* man)
{
  bool isAdded;

  if (!db)
    return false;

  uecho_mutex_lock(db->mutex);
  isAdded = uecho_manufacturelist_add(db->mans, man);
  uecho_mutex_unlock(db->mutex);

  return isAdded;
}

/****************************************
 * uecho_database_addobject
 ****************************************/

bool uecho_database_addobject(uEchoDatabase* db, uEchoObject* obj)
{
  bool isAdded;

  if (!db)
    return false;

  uecho_mutex_lock(db->mutex);
  isAdded = uecho_objectlist_add(db->objs, obj);
  uecho_mutex_unlock(db->mutex);

  return isAdded;
}

/****************************************
 * uecho_database_newstandardmanufacture
 ****************************************/

static uEchoManufacture* uecho_database_newstandardmanufacture(const uEchoStdManufacture* stdMan)
{
  uEchoManufacture* man;

  man = uecho_manufacture_new();
  if (!man)
    return NULL;

  uecho_manufacture_setcode(man, stdMan->code);
  uecho_manufacture_setname(man, stdMan->name);

  return man;
}

/****************************************
 * uecho_database_getmanufacture
 ****************************************/

uEchoManufacture* uecho_database_getmanufacture(uEchoDatabase* db, uEchoManufactureCode code)
{
  uEchoManufacture* man;
  const uEchoStdManufacture* stdMan;

  if (!db)
    return NULL;

  uecho_mutex_lock(db->mutex);

  man = uecho_manufacturelist_getbycode(db->mans, code);

  // The standard database keeps only the entries that have been looked up.
  if (!man && db->isStandard) {
    stdMan = uecho_std_getmanufacture(code);
    if (stdMan) {
      man = uecho_database_newstandardmanufacture(stdMan);
      if (man && !uecho_manufacturelist_add(db->mans, man)) {
        uecho_manufacture_delete(man);
        man = NULL;
      }
    }
  }

  uecho_mutex_unlock(db->mutex);

  return man;
}

/****************************************
 * uecho_database_newstandardobject
 ****************************************/

static uEchoObject* uecho_database_newstandardobject(const uEchoStdObject* stdObj)
{
  uEchoObject* obj;
  uEchoProperty* prop;
  size_t n;

  obj = uecho_object_new();
  if (!obj)
    return NULL;

  obj->code[0] = stdObj->grpCode;
  obj->code[1] = stdObj->clsCode;
  uecho_object_setname(obj, stdObj->name);

  for (n = 0; n < stdObj->propCnt; n++) {
    prop = uecho_property_new();
    if (!prop) {
      uecho_object_delete(obj);
      return NULL;
    }
    uecho_property_setcode(prop, stdObj->props[n].code);
    uecho_property_setname(prop, stdObj->props[n].name);
    uecho_property_setattribute(prop, stdObj->props[n].attr);
    if (!uecho_object_addproperty(obj, prop)) {
      uecho_property_delete(prop);
      uecho_object_delete(obj);
      return NULL;
    }
  }

  return obj;
}

/****************************************
 * uecho_database_getobject
 ****************************************/

uEchoObject* uecho_database_getobject(uEchoDatabase* db, byte grpCode, byte clsCode)
{
  uEchoObject* obj;
  const uEchoStdObject* stdObj;

  if (!db)
    return NULL;

  uecho_mutex_lock(db->mutex);

  obj = uecho_objectlist_getbygroupclasscode(db->objs, grpCode, clsCode);

  // The standard database keeps only the entries that have been looked up.
  if (!obj && db->isStandard) {
    stdObj = uecho_std_getobject(grpCode, clsCode);
    if (stdObj) {
      obj = uecho_database_newstandardobject(stdObj);
      if (obj && !uecho_objectlist_add(db->objs, obj)) {
        uecho_object_delete(obj);
        obj = NULL;
      }
    }
  }

  uecho_mutex_unlock(db->mutex);

  return obj;
}

/****************************************
 * uecho_std_getobject
 ****************************************/

const uEchoStdObject* uecho_std_getobject(byte grpCode, byte clsCode)
{
  size_t low, high, mid;
  int key, midKey;

  key = (grpCode << 8) | clsCode;
  low = 0;
  high = uecho_std_objectcount;
  while (low < high) {
    mid = low + (high - low) / 2;
    midKey = (uecho_std_objects[mid].grpCode << 8) | uecho_std_objects[mid].clsCode;
    if (midKey == key)
      return &uecho_std_objects[mid];
    if (midKey < key)
      low = mid + 1;
    else
      high = mid;
  }

  return NULL;
}

/****************************************
 * uecho_std_object_getproperty
 ****************************************/

const uEchoStdProperty* uecho_std_object_getproperty(const uEchoStdObject* stdObj, byte epc)
{
  size_t n;

  if (!stdObj)
    return NULL;

  for (n = 0; n < stdObj->propCnt; n++) {
    if (stdObj->props[n].code == epc)
      return &stdObj->props[n];
  }

  return NULL;
}

/****************************************
 * uecho_std_getmanufacture
 ****************************************/

const uEchoStdManufacture* uecho_std_getmanufacture(int code)
{
  size_t low, high, mid;

  low = 0;
  high = uecho_std_manufacturecount;
  while (low < high) {
    mid = low + (high - low) / 2;
    if (uecho_std_manufactures[mid].code == code)
      return &uecho_std_manufactures[mid];
    if (uecho_std_manufactures[mid].code < code)
      low = mid + 1;
    else
      high = mid;
  }

  return NULL;
}

/****************************************
 * uecho_std_getobjectname
 ****************************************/

const char* uecho_std_getobjectname(byte grpCode, byte clsCode)
{
  const uEchoStdObject* stdObj;

  stdObj = uecho_std_getobject(grpCode, clsCode);
  if (!stdObj)
    return NULL;

  return stdObj->name;
}

/****************************************
 * uecho_std_getpropertyname
 ****************************************/

const char* uecho_std_getpropertyname(byte grpCode, byte clsCode, byte epc)
{
  const uEchoStdProperty* stdProp;

  // Standard properties are added from the super class first, so its
  // definitions take precedence over the class ones.
  stdProp = uecho_std_object_getproperty(uecho_std_getobject(0x00, 0x00), epc);
  if (!stdProp)
    stdProp = uecho_std_object_getproperty(uecho_std_getobject(grpCode, clsCode), epc);
  if (!stdProp)
    return NULL;

  return stdProp->name;
}
