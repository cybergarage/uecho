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

#include <uecho/std/_database.h>

#include <uecho/device.h>
#include <uecho/util/strings.h>

BOOST_AUTO_TEST_CASE(DatabaseStandardManufacture)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_REQUIRE(db);

  uEchoManufactureCode manCodes[] = { 0x00000B, 0x000005 };
  const char* manNames[] = { "Panasonic", "Sharp" };
  for (int n = 0; n < (sizeof(manCodes) / sizeof(uEchoManufactureCode)); n++) {
    uEchoManufacture* man = uecho_database_getmanufacture(db, manCodes[n]);
#if defined(UECHO_DATABASE_NONE)
    BOOST_REQUIRE(!man);
#else
    BOOST_REQUIRE(man);
    BOOST_REQUIRE_EQUAL(uecho_strncmp(uecho_manufacture_getname(man), manNames[n], uecho_strlen(manNames[n])), 0);
    // The same entry is returned for a repeated lookup.
    BOOST_REQUIRE_EQUAL(uecho_database_getmanufacture(db, manCodes[n]), man);
#endif
  }

  BOOST_REQUIRE(!uecho_database_getmanufacture(db, 0x123456));
}

BOOST_AUTO_TEST_CASE(DatabaseStandardSuperObject)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_REQUIRE(db);

  byte objGroupCode = 0x00;
  byte objClassCode = 0x00;

  uEchoObject* obj = uecho_database_getobject(db, objGroupCode, objClassCode);
  BOOST_REQUIRE(obj);

  byte objPropCodes[] = {
    0x80
  };
  uEchoPropertyAttr objPropAttrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrWrite | uEchoPropertyAttrAnnoRequired),
  };
  for (int n = 0; n < (sizeof(objPropCodes) / sizeof(byte)); n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, objPropCodes[n]);
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getattribute(prop), objPropAttrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardNodeProfile)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_REQUIRE(db);

  byte objGroupCode = 0x0E;
  byte objClassCode = 0xF0;

  uEchoObject* obj = uecho_database_getobject(db, objGroupCode, objClassCode);
  BOOST_REQUIRE(obj);

  byte objPropCodes[] = {
    0x80
  };
  uEchoPropertyAttr objPropAttrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrAnnoRequired),
  };
  for (int n = 0; n < (sizeof(objPropCodes) / sizeof(byte)); n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, objPropCodes[n]);
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getattribute(prop), objPropAttrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardDevice)
{
  uEchoDatabase* db = uecho_standard_getdatabase();
  BOOST_REQUIRE(db);

  byte objGroupCode = 0x02;
  byte objClassCode = 0x91;

  uEchoObject* obj = uecho_database_getobject(db, objGroupCode, objClassCode);
#if defined(UECHO_DATABASE_NONE)
  BOOST_REQUIRE(!obj);
  return;
#endif
  BOOST_REQUIRE(obj);
  BOOST_REQUIRE_EQUAL(uecho_object_getname(obj), "Mono functional lighting");

  byte objPropCodes[] = {
    0x80
  };
  uEchoPropertyAttr objPropAttrs[] = {
    uEchoPropertyAttr(uEchoPropertyAttrReadRequired | uEchoPropertyAttrWriteRequired | uEchoPropertyAttrAnnoRequired),
  };
  for (int n = 0; n < (sizeof(objPropCodes) / sizeof(byte)); n++) {
    uEchoProperty* prop = uecho_object_getproperty(obj, objPropCodes[n]);
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getattribute(prop), objPropAttrs[n]);
  }
}

BOOST_AUTO_TEST_CASE(DatabaseStandardConditionalRules)
{
#if !defined(UECHO_DATABASE_NONE)
  // Route B Identification number is "required_c" for get, so it is readable
  // but not mandatory.
  const uEchoStdProperty* stdProp = uecho_std_object_getproperty(uecho_std_getobject(0x02, 0x88), 0xC0);
  BOOST_REQUIRE(stdProp);
  BOOST_REQUIRE_EQUAL(stdProp->attr, (byte)(uEchoPropertyAttrRead | uEchoPropertyAttrAnno));
#endif
}

// Checks the invariants the lookups rely on, so that a database regenerated
// from a new MRA or manufacturer code list cannot silently break them.
BOOST_AUTO_TEST_CASE(DatabaseStandardTables)
{
  BOOST_REQUIRE(0 < uecho_std_objectcount);
  for (size_t n = 0; n < uecho_std_objectcount; n++) {
    const uEchoStdObject* stdObj = &uecho_std_objects[n];
    if (0 < n) {
      const uEchoStdObject* prevObj = &uecho_std_objects[n - 1];
      BOOST_REQUIRE_LT((prevObj->grpCode << 8) | prevObj->clsCode, (stdObj->grpCode << 8) | stdObj->clsCode);
    }
    BOOST_REQUIRE(stdObj->name);
    BOOST_REQUIRE(stdObj->props || stdObj->propCnt == 0);
    BOOST_REQUIRE_EQUAL(uecho_std_getobject(stdObj->grpCode, stdObj->clsCode), stdObj);
    for (size_t i = 0; i < stdObj->propCnt; i++) {
      BOOST_REQUIRE_GE(stdObj->props[i].code, 0x80);
      BOOST_REQUIRE(stdObj->props[i].name);
      for (size_t j = 0; j < i; j++)
        BOOST_REQUIRE_NE(stdObj->props[j].code, stdObj->props[i].code);
    }
  }

  // The library needs these classes in every configuration.
  const uEchoStdObject* superObj = uecho_std_getobject(0x00, 0x00);
  BOOST_REQUIRE(superObj);
  BOOST_REQUIRE(uecho_std_object_getproperty(superObj, 0x80));
  const uEchoStdObject* nodeProfObj = uecho_std_getobject(0x0E, 0xF0);
  BOOST_REQUIRE(nodeProfObj);
  BOOST_REQUIRE(uecho_std_object_getproperty(nodeProfObj, 0xD6));

#if defined(UECHO_DATABASE_NONE)
  BOOST_REQUIRE_EQUAL(uecho_std_objectcount, 2);
  BOOST_REQUIRE_EQUAL(uecho_std_manufacturecount, 0);
#else
  BOOST_REQUIRE(uecho_std_getobject(0x02, 0x91));
  BOOST_REQUIRE(0 < uecho_std_manufacturecount);
#endif

  for (size_t n = 0; n < uecho_std_manufacturecount; n++) {
    if (0 < n)
      BOOST_REQUIRE_LT(uecho_std_manufactures[n - 1].code, uecho_std_manufactures[n].code);
    BOOST_REQUIRE(uecho_std_manufactures[n].name);
    BOOST_REQUIRE_EQUAL(uecho_std_getmanufacture(uecho_std_manufactures[n].code), &uecho_std_manufactures[n]);
  }
  BOOST_REQUIRE(!uecho_std_getobject(0x0F, 0xFF));
  BOOST_REQUIRE(!uecho_std_getmanufacture(0x123456));
}

BOOST_AUTO_TEST_CASE(DatabaseStandardNames)
{
  uEchoObject* obj = uecho_device_new();
  BOOST_REQUIRE(obj);
  uecho_object_setcode(obj, 0x029101);

  // Standard names are looked up from the tables, not copied into objects.
  uEchoProperty* prop = uecho_object_getproperty(obj, 0x80);
  BOOST_REQUIRE(prop);
  BOOST_REQUIRE_EQUAL(uecho_property_getname(prop), "Operation status");
#if defined(UECHO_DATABASE_NONE)
  BOOST_REQUIRE(!uecho_object_getname(obj));
  BOOST_REQUIRE(!uecho_object_getproperty(obj, 0xB0));
  // Applications add the class properties they implement.
  BOOST_REQUIRE(uecho_object_setproperty(obj, 0xB0, uEchoPropertyAttrReadWrite));
  prop = uecho_object_getproperty(obj, 0xB0);
  BOOST_REQUIRE(prop);
  BOOST_REQUIRE(uecho_property_isreadable(prop));
  BOOST_REQUIRE(uecho_property_iswritable(prop));
  BOOST_REQUIRE(!uecho_property_getname(prop));
#else
  BOOST_REQUIRE_EQUAL(uecho_object_getname(obj), "Mono functional lighting");
  prop = uecho_object_getproperty(obj, 0xB0);
  BOOST_REQUIRE(prop);
  BOOST_REQUIRE_EQUAL(uecho_property_getname(prop), "Light level Setting");
#endif

  // An explicitly set name takes precedence.
  uecho_object_setname(obj, "my light");
  BOOST_REQUIRE_EQUAL(uecho_object_getname(obj), "my light");
  prop = uecho_object_getproperty(obj, 0x80);
  uecho_property_setname(prop, "power");
  BOOST_REQUIRE_EQUAL(uecho_property_getname(prop), "power");

  // A copied property keeps an explicit name.
  uEchoProperty* copyProp = uecho_property_copy(prop);
  BOOST_REQUIRE(copyProp);
  BOOST_REQUIRE_EQUAL(uecho_property_getname(copyProp), "power");
  uecho_property_delete(copyProp);

  // A property without a parent object has no standard name.
  uEchoProperty* orphanProp = uecho_property_new();
  uecho_property_setcode(orphanProp, 0x80);
  BOOST_REQUIRE(!uecho_property_getname(orphanProp));
  uecho_property_delete(orphanProp);

  uecho_object_delete(obj);
}
