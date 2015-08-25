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

#include <uecho/core/message.h>

BOOST_AUTO_TEST_CASE(MessageBasicFunctions)
{
  uEchoMessage *msg = uecho_message_new();
  
  uecho_message_settid(msg, uEchoEhd1);
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), uEchoEhd1);

  uecho_message_setehd2(msg, uEchoEhd2);
  BOOST_CHECK_EQUAL(uecho_message_getehd2(msg), uEchoEhd2);
  
  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageTid)
{
  uEchoMessage *msg = uecho_message_new();
  
  BOOST_CHECK(uecho_message_settid(msg, uEchoTidMin));
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), uEchoTidMin);
  
  BOOST_CHECK(uecho_message_settid(msg, uEchoTidMax));
  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), uEchoTidMax);
  
  srand((int)time(NULL));
  for (int n=0; n<100; n++) {
    int tid = rand() % uEchoTidMax;
    BOOST_CHECK(uecho_message_settid(msg, tid));
    BOOST_CHECK_EQUAL(uecho_message_gettid(msg), tid);
  }
  
  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageBadDataLength)
{
  uEchoMessage *msg = uecho_message_new();
  
  for (int n=0; n<uEchoMessageMinLen; n++) {
    BOOST_CHECK(!uecho_message_parse(msg, NULL, n));
  }
  
  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageBadHeader)
{
  uEchoMessage *msg = uecho_message_new();
  
  byte msgBytes[] = {
    0x00,
    0x00,
    0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
    0x00,
    0x00,
  };
  
  BOOST_CHECK(!uecho_message_parse(msg, msgBytes, uEchoMessageMinLen));
  
  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageRequest)
{
  uEchoMessage *msg = uecho_message_new();

  byte msgBytes[] = {
    uEchoEhd1,
    uEchoEhd2,
    0x00, 0x00,
    0xA0, 0xB0, 0xC0,
    0xD0, 0xE0, 0xF0,
    uEchoEsvReadRequest,
    3,
    1, 1, 'a',
    2, 2, 'b', 'c',
    3, 3, 'c', 'd', 'e',
  };
  
  BOOST_CHECK(uecho_message_parse(msg, msgBytes, sizeof(msgBytes)));

  BOOST_CHECK_EQUAL(uecho_message_gettid(msg), 0);

  BOOST_CHECK_EQUAL(uecho_message_getsourceobjectcode(msg), 0xA0B0C0);
  BOOST_CHECK_EQUAL(uecho_message_getdestinationobjectcode(msg), 0xD0E0F0);
  
  BOOST_CHECK_EQUAL(uecho_message_getesv(msg), uEchoEsvReadRequest);
  
  BOOST_CHECK_EQUAL(uecho_message_getopc(msg), 3);
  
  for (int n=1; n<=uecho_message_getopc(msg); n++) {
    uEchoProperty *prop = uecho_message_getproperty(msg, (n-1));
    BOOST_CHECK(prop);
    BOOST_CHECK_EQUAL(uecho_property_getcode(prop), n);
    BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), n);
    byte *data = uecho_property_getdata(prop);
    BOOST_CHECK(data);
    for (int i=0; i<uecho_property_getdatasize(prop); i++) {
      BOOST_CHECK_EQUAL(data[i], 'a' + (n-1) + i);
    }
  }
  
  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageSearch)
{
  uEchoMessage *msg = uecho_message_search_new();

  BOOST_CHECK(uecho_message_issearchrequest(msg));
  BOOST_CHECK_EQUAL(uecho_message_size(msg), 14);
  
  uecho_message_delete(msg);
}
