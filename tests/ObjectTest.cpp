/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) Satoshi Konno 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

#include <boost/test/unit_test.hpp>

#include <uecho/object.h>

BOOST_AUTO_TEST_CASE(ObjectBasicFunctions)
{
  uEchoObject *obj = uecho_object_new();
  
  srand((int)time(NULL));
  for (int n=0; n<10; n++) {
    byte code;
    
    code = rand() % 255;
    uecho_object_setclassgroupcode(obj, code);
    BOOST_CHECK_EQUAL(uecho_object_getclassgroupcode(obj), code);

    code = rand() % 255;
    uecho_object_setclasscode(obj, code);
    BOOST_CHECK_EQUAL(uecho_object_getclasscode(obj), code);

    code = rand() % 255;
    uecho_object_setinstancecode(obj, code);
    BOOST_CHECK_EQUAL(uecho_object_getinstancecode(obj), code);
  }
  
  uecho_object_delete(obj);
}
