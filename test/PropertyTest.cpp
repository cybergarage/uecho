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
#include <string>
#include <vector>

#include <uecho/_property.h>

BOOST_AUTO_TEST_CASE(PropertyBasicFunctions)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_CHECK(prop);

  for (int n = uEchoPropertyCodeMin; n < uEchoPropertyCodeMax; n++) {
    uecho_property_setcode(prop, n);
    BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
  }

  BOOST_CHECK(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertySetData)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  uecho_property_cleardata(prop);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  uecho_property_setdata(prop, NULL, 0);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  std::vector<std::string> test_data_vec;
  test_data_vec.push_back("a");
  test_data_vec.push_back("abcd");
  test_data_vec.push_back("abcd0123456789");

  for (int i = 0; i < test_data_vec.size(); i++) {
    const char* test_data = test_data_vec[i].c_str();
    size_t test_data_len = strlen(test_data);
    uecho_property_setdata(prop, (const byte*)test_data, test_data_len);
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), test_data_len);
    byte* prop_data = uecho_property_getdata(prop);
    BOOST_CHECK(prop_data);
    for (int n = 0; n < test_data_len; n++) {
      BOOST_CHECK_EQUAL(test_data[n], prop_data[n]);
    }
  }

  uecho_property_cleardata(prop);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);
  BOOST_CHECK_EQUAL(uecho_property_getdata(prop), (byte*)NULL);

  BOOST_CHECK(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyPermission)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrNone);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrRead);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrReadRequired);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrWrite);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrWriteRequired);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrReadWrite);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnno);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnnoRequired);
  BOOST_CHECK_EQUAL(uecho_property_isreadable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadrequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswritable(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriterequired(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isreadonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_iswriteonly(prop), false);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), true);
  BOOST_CHECK_EQUAL(uecho_property_isannouncerequired(prop), true);

  BOOST_CHECK(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyAnnouncement)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);

  uecho_property_setattribute(prop, uEchoPropertyAttrAnno);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), true);

  uecho_property_setattribute(prop, uEchoPropertyAttrNone);
  BOOST_CHECK_EQUAL(uecho_property_isannounceable(prop), false);

  BOOST_CHECK(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyByte2Integer)
{
  uEchoProperty* prop = uecho_property_new();

  int val;
  for (int n = 0; n <= 0xFF; n++) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 1));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 1, &val));
    BOOST_CHECK_EQUAL(n, val);
  }

  for (int n = 0; n <= 0xFFFF; n += (0xFFFF / 0xFF)) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 2));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 2, &val));
    BOOST_CHECK_EQUAL(n, val);
  }

  for (int n = 0; n <= 0xFFFFFF; n += (0xFFFFFF / 0xFF)) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 3));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 3, &val));
    BOOST_CHECK_EQUAL(n, val);
  }

  for (int n = 0; n < 0xFFFFFFFF; n += (0xFFFFFFFF / 0xFF)) {
    BOOST_CHECK(uecho_property_setintegerdata(prop, n, 4));
    BOOST_CHECK(uecho_property_getintegerdata(prop, 4, &val));
    BOOST_CHECK_EQUAL(n, val);
  }

  BOOST_CHECK(uecho_property_delete(prop));
}

BOOST_AUTO_TEST_CASE(PropertyAddData)
{
  uEchoProperty* prop = uecho_property_new();

  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);

  const size_t property_add_data_count = 10;

  for (size_t n = 0; n < property_add_data_count; n++) {
    byte new_byte = n;
    BOOST_CHECK(uecho_property_addbytedata(prop, new_byte));
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), (n + 1));
    byte* bytes = uecho_property_getdata(prop);
    BOOST_CHECK(bytes);
    BOOST_CHECK_EQUAL(bytes[n], new_byte);
  }

  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), property_add_data_count);

  BOOST_CHECK(uecho_property_delete(prop));
}
