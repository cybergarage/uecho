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
  uEchoObject* obj = uecho_profile_new();
  BOOST_REQUIRE(obj);
  uecho_object_delete(obj);
}

BOOST_AUTO_TEST_CASE(NodeProfileObjectMandatoryProperties)
{
  uEchoObject* obj = uecho_nodeprofile_new();

  BOOST_REQUIRE(uecho_object_iscode(obj, uEchoNodeProfileObject));

  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoProfileManufacturerCode));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoProfileAnnoPropertyMap));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoProfileSetPropertyMap));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoProfileGetPropertyMap));

  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassOperatingStatus));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassVersionInformation));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassIdentificationNumber));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassNumberOfSelfNodeInstances));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassNumberOfSelfNodeClasses));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassInstanceListNotification));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassSelfNodeInstanceListS));
  BOOST_REQUIRE(uecho_object_hasproperty(obj, uEchoNodeProfileClassSelfNodeClassListS));

  BOOST_REQUIRE(uecho_nodeprofile_isoperatingstatus(obj));

  uecho_object_delete(obj);
}
