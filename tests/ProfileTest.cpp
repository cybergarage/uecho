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

#include <uecho/profile.h>

BOOST_AUTO_TEST_CASE(ProfileSuperClassProperties)
{
  uEchoObject *obj = uecho_profile_new();
  
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileSuperClassManufacturerCode));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileSuperClassAnnoPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileSuperClassSetPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileSuperClassGetPropertyMap));
  
  uecho_object_delete(obj);
}
