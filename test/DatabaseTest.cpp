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

BOOST_AUTO_TEST_CASE(StandardManufactureDatabase)
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

BOOST_AUTO_TEST_CASE(StandardObjectDatabase)
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
