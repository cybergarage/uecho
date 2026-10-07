/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#ifndef _UECHO_STD_STANDARD_INTERNAL_H_
#define _UECHO_STD_STANDARD_INTERNAL_H_

#include <stddef.h>

#include <uecho/typedef.h>

#include <uecho/_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/****************************************
 * Data Type
 ****************************************/

// The standard object and manufacturer databases are const tables generated
// by objects_mra.pl and manufacturers.pl, so they are placed in ROM (flash on
// MCUs). With UECHO_DATABASE_NONE, only the super class and the node profile
// class are compiled in.

typedef struct {
  byte code;
  byte attr;
  const char* name;
#if defined(UECHO_DATABASE_VALUE_METADATA) && UECHO_DATABASE_VALUE_METADATA
  // Original MRA data schema JSON. Internal layout requires consistent build flags.
  const char* valueSchema;
#endif
} uEchoStdProperty;

typedef struct {
  byte grpCode;
  byte clsCode;
  const char* name;
  const uEchoStdProperty* props;
  size_t propCnt;
} uEchoStdObject;

typedef struct {
  int code;
  const char* name;
} uEchoStdManufacture;

// Sorted by group and class code.
extern const uEchoStdObject uecho_std_objects[];
extern const size_t uecho_std_objectcount;
// One shared MRA definitions dictionary; NULL in metadata-disabled builds.
extern const char* const uecho_std_value_definitions;
extern const char* const uecho_std_source_version;

// Sorted by manufacturer code.
extern const uEchoStdManufacture uecho_std_manufactures[];
extern const size_t uecho_std_manufacturecount;

/****************************************
 * Function (Standard tables)
 ****************************************/

const uEchoStdObject* uecho_std_getobject(byte grpCode, byte clsCode);
const uEchoStdProperty* uecho_std_object_getproperty(const uEchoStdObject* stdObj, byte epc);
const uEchoStdManufacture* uecho_std_getmanufacture(int code);

const char* uecho_std_getobjectname(byte grpCode, byte clsCode);
const char* uecho_std_getpropertyname(byte grpCode, byte clsCode, byte epc);

bool uecho_object_addstandardobjectproperties(uEchoObject* obj, const uEchoStdObject* stdObj);

#ifdef __cplusplus
} /* extern C */
#endif

#endif /* _UECHO_STD_STANDARD_INTERNAL_H_ */
