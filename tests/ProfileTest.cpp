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

BOOST_AUTO_TEST_CASE(ProfileSuperClassMandatoryProperties)
{
  uEchoObject *obj = uecho_profile_new();
  
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileManufacturerCode));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileAnnoPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileSetPropertyMap));
  BOOST_CHECK(uecho_object_hasproperty(obj, uEchoProfileGetPropertyMap));
  
  uecho_object_delete(obj);
}
