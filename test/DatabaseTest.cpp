/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>

#include <uecho/std/database.h>
#include <uecho/util/strings.h>

BOOST_AUTO_TEST_CASE(DatabaseStandardManufacture)
{
  uEchoDatabase *db = uecho_standard_getdatabase();
  BOOST_CHECK(db);
  
  uEchoManufactureCode manCodes[] = {0x00000B, 0x000005};
  const char *manNames[] = {"Panasonic", "Sharp"};
  for (int n=0; n<(sizeof(manCodes)/sizeof(uEchoManufactureCode)); n++) {
    uEchoManufacture *man = uecho_database_getmanufacture(db, manCodes[n]);
    BOOST_CHECK(man);
    BOOST_CHECK_EQUAL(uecho_strncmp(uecho_manufacture_getname(man), manNames[n], uecho_strlen(manNames[n])), 0);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardSuperObject)
{
  uEchoDatabase *db = uecho_standard_getdatabase();
  BOOST_CHECK(db);
  
  byte objGroupCode = 0x00;
  byte objClassCode = 0x00;

  uEchoObject *obj = uecho_database_getobject(db, objGroupCode, objClassCode);
  BOOST_CHECK(obj);

  byte objPropCodes[] = {
    0x80
  };
  uEchoPropertyAttr objPropAttrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrWrite | uEchoPropertyAttrAnnoRequired),
  };
  for (int n=0; n<(sizeof(objPropCodes)/sizeof(byte)); n++) {
    uEchoProperty *prop = uecho_object_getproperty(obj, objPropCodes[n]);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getattribute(prop), objPropAttrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardNodeProfile)
{
  uEchoDatabase *db = uecho_standard_getdatabase();
  BOOST_CHECK(db);
  
  byte objGroupCode = 0x0E;
  byte objClassCode = 0xF0;

  uEchoObject *obj = uecho_database_getobject(db, objGroupCode, objClassCode);
  BOOST_CHECK(obj);

  byte objPropCodes[] = {
    0x80
  };
  uEchoPropertyAttr objPropAttrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrAnnoRequired),
  };
  for (int n=0; n<(sizeof(objPropCodes)/sizeof(byte)); n++) {
    uEchoProperty *prop = uecho_object_getproperty(obj, objPropCodes[n]);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getattribute(prop), objPropAttrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardDevice)
{
  uEchoDatabase *db = uecho_standard_getdatabase();
  BOOST_CHECK(db);
  
  byte objGroupCode = 0x02;
  byte objClassCode = 0x91;

  uEchoObject *obj = uecho_database_getobject(db, objGroupCode, objClassCode);
  BOOST_CHECK(obj);

  byte objPropCodes[] = {
    0x80
  };
  uEchoPropertyAttr objPropAttrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrWriteRequired | uEchoPropertyAttrAnnoRequired),
  };
  for (int n=0; n<(sizeof(objPropCodes)/sizeof(byte)); n++) {
    uEchoProperty *prop = uecho_object_getproperty(obj, objPropCodes[n]);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getattribute(prop), objPropAttrs[n]);
  }
}
