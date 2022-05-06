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
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_CHECK(db);

  uEchoManufactureCode man_codes[] = { 0x00000B, 0x000005 };
  const char* man_names[] = { "Panasonic", "Sharp" };
  for (int n = 0; n < (sizeof(man_codes) / sizeof(uEchoManufactureCode)); n++) {
    uEchoManufacture* man = uecho_database_getmanufacture(db, man_codes[n]);
    BOOST_CHECK(man);
    BOOST_CHECK_EQUAL(uecho_strncmp(uecho_manufacture_getname(man), man_names[n], uecho_strlen(man_names[n])), 0);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardSuperObject)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_CHECK(db);

  byte obj_group_code = 0x00;
  byte obj_class_code = 0x00;

  uEchoObject* obj = uecho_database_getobject(db, obj_group_code, obj_class_code);
  BOOST_CHECK(obj);

  byte obj_prop_codes[] = {
    0x80
  };
  uEchoPropertyAttr obj_prop_attrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrWrite | uEchoPropertyAttrAnnoRequired),
  };
  for (int n = 0; n < (sizeof(obj_prop_codes) / sizeof(byte)); n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, obj_prop_codes[n]);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getattribute(prop), obj_prop_attrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardNodeProfile)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_CHECK(db);

  byte obj_group_code = 0x0E;
  byte obj_class_code = 0xF0;

  uEchoObject* obj = uecho_database_getobject(db, obj_group_code, obj_class_code);
  BOOST_CHECK(obj);

  byte obj_prop_codes[] = {
    0x80
  };
  uEchoPropertyAttr obj_prop_attrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrAnnoRequired),
  };
  for (int n = 0; n < (sizeof(obj_prop_codes) / sizeof(byte)); n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, obj_prop_codes[n]);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getattribute(prop), obj_prop_attrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardDevice)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_CHECK(db);

  byte obj_group_code = 0x02;
  byte obj_class_code = 0x91;

  uEchoObject* obj = uecho_database_getobject(db, obj_group_code, obj_class_code);
  BOOST_CHECK(obj);

  byte obj_prop_codes[] = {
    0x80
  };
  uEchoPropertyAttr obj_prop_attrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrWriteRequired | uEchoPropertyAttrAnnoRequired),
  };
  for (int n = 0; n < (sizeof(obj_prop_codes) / sizeof(byte)); n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, obj_prop_codes[n]);
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getattribute(prop), obj_prop_attrs[n]);
  }
}
