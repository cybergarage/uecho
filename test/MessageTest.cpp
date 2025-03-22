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

#include <uecho/_message.h>

BOOST_AUTO_TEST_CASE(MessageBasicFunctions)
{
  uEchoMessage* msg = uecho_message_new();

  uecho_message_settid(msg, uEchoEhd1);
  BOOST_REQUIRE_EQUAL(uecho_message_gettid(msg), (uEchoTID)uEchoEhd1);

  uecho_message_setehd2(msg, uEchoEhd2);
  BOOST_REQUIRE_EQUAL(uecho_message_getehd2(msg), (uEchoTID)uEchoEhd2);

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageTid)
{
  uEchoMessage* msg = uecho_message_new();

  BOOST_REQUIRE(uecho_message_settid(msg, uEchoTidMin));
  BOOST_REQUIRE_EQUAL(uecho_message_gettid(msg), (uEchoTID)uEchoTidMin);

  BOOST_REQUIRE(uecho_message_settid(msg, uEchoTidMax));
  BOOST_REQUIRE_EQUAL(uecho_message_gettid(msg), (uEchoTID)uEchoTidMax);

  srand((int)time(NULL));
  for (int n = 0; n < 100; n++) {
    int tid = rand() % uEchoTidMax;
    BOOST_REQUIRE(uecho_message_settid(msg, tid));
    BOOST_REQUIRE_EQUAL(uecho_message_gettid(msg), tid);
  }

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageBadDataLength)
{
  uEchoMessage* msg = uecho_message_new();

  for (int n = 0; n < uEchoMessageMinLen; n++) {
    BOOST_REQUIRE(!uecho_message_parse(msg, NULL, n));
  }

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageBadHeader)
{
  uEchoMessage* msg = uecho_message_new();

  byte msgBytes[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
  };

  BOOST_REQUIRE(!uecho_message_parse(msg, msgBytes, uEchoMessageMinLen));

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageRequest)
{
  uEchoMessage* msg = uecho_message_new();

  byte msgBytes[] = {
    uEchoEhd1,
    uEchoEhd2,
    0x00,
    0x00,
    0xA0,
    0xB0,
    0xC0,
    0xD0,
    0xE0,
    0xF0,
    uEchoEsvReadRequest,
    3,
    1,
    1,
    'a',
    2,
    2,
    'b',
    'c',
    3,
    3,
    'c',
    'd',
    'e',
  };

  BOOST_REQUIRE(uecho_message_parse(msg, msgBytes, sizeof(msgBytes)));

  BOOST_REQUIRE_EQUAL(uecho_message_gettid(msg), 0);

  BOOST_REQUIRE_EQUAL(uecho_message_getsourceobjectcode(msg), 0xA0B0C0);
  BOOST_REQUIRE_EQUAL(uecho_message_getdestinationobjectcode(msg), 0xD0E0F0);

  BOOST_REQUIRE_EQUAL(uecho_message_getesv(msg), (uEchoEsv)uEchoEsvReadRequest);

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(msg), 3);
  BOOST_REQUIRE_EQUAL(uecho_message_getopcset(msg), 0);
  BOOST_REQUIRE_EQUAL(uecho_message_getopcget(msg), 0);

  for (int n = 1; n <= uecho_message_getopc(msg); n++) {
    uEchoProperty* prop = uecho_message_getproperty(msg, (n - 1));
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), n);
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), n);
    byte* data = uecho_property_getdata(prop);
    BOOST_REQUIRE(data);
    for (int i = 0; i < uecho_property_getdatasize(prop); i++) {
      BOOST_REQUIRE_EQUAL(data[i], 'a' + (n - 1) + i);
    }
  }

  uEchoMessage* msgCopy = uecho_message_new();
  BOOST_REQUIRE(uecho_message_parse(msgCopy, uecho_message_getbytes(msg), uecho_message_size(msg)));
  BOOST_REQUIRE(uecho_message_equals(msg, msgCopy));
  uecho_message_delete(msgCopy);

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageWriteReadRequest)
{
  uEchoMessage* msg = uecho_message_new();

  byte msgBytes[] = {
    uEchoEhd1,
    uEchoEhd2,
    0x00,
    0x00,
    0xA0,
    0xB0,
    0xC0,
    0xD0,
    0xE0,
    0xF0,
    uEchoEsvWriteReadRequest,
    3,
    1,
    1,
    'a',
    2,
    2,
    'b',
    'c',
    3,
    3,
    'c',
    'd',
    'e',
    3,
    1,
    1,
    'v',
    2,
    2,
    'w',
    'x',
    3,
    3,
    'x',
    'y',
    'z',
  };

  BOOST_REQUIRE(uecho_message_parse(msg, msgBytes, sizeof(msgBytes)));

  BOOST_REQUIRE_EQUAL(uecho_message_gettid(msg), 0);

  BOOST_REQUIRE_EQUAL(uecho_message_getsourceobjectcode(msg), 0xA0B0C0);
  BOOST_REQUIRE_EQUAL(uecho_message_getdestinationobjectcode(msg), 0xD0E0F0);

  BOOST_REQUIRE_EQUAL(uecho_message_getesv(msg), (uEchoEsv)uEchoEsvWriteReadRequest);

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(msg), 0);
  BOOST_REQUIRE_EQUAL(uecho_message_getopcset(msg), 3);
  BOOST_REQUIRE_EQUAL(uecho_message_getopcget(msg), 3);

  for (int n = 1; n <= uecho_message_getopcset(msg); n++) {
    uEchoProperty* prop = uecho_message_getpropertyset(msg, (n - 1));
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), n);
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), n);
    byte* data = uecho_property_getdata(prop);
    BOOST_REQUIRE(data);
    for (int i = 0; i < uecho_property_getdatasize(prop); i++) {
      BOOST_REQUIRE_EQUAL(data[i], 'a' + (n - 1) + i);
    }
  }

  for (int n = 1; n <= uecho_message_getopcget(msg); n++) {
    uEchoProperty* prop = uecho_message_getpropertyget(msg, (n - 1));
    BOOST_REQUIRE(prop);
    BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), n);
    BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), n);
    byte* data = uecho_property_getdata(prop);
    BOOST_REQUIRE(data);
    for (int i = 0; i < uecho_property_getdatasize(prop); i++) {
      BOOST_REQUIRE_EQUAL(data[i], 'v' + (n - 1) + i);
    }
  }

  uEchoMessage* msgCopy = uecho_message_new();
  BOOST_REQUIRE(uecho_message_parse(msgCopy, uecho_message_getbytes(msg), uecho_message_size(msg)));
  BOOST_REQUIRE(uecho_message_equals(msg, msgCopy));
  uecho_message_delete(msgCopy);

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageEsvType)
{
  uEchoMessage* msg = uecho_message_new();

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvWriteRequest));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvWriteRequestResponseRequired));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvReadRequest));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvNotificationRequest));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvWriteReadRequest));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvWriteResponse));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvReadResponse));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvNotification));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), true);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvNotificationResponseRequired));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvNotificationResponse));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), true);

  BOOST_REQUIRE(uecho_message_setesv(msg, uEchoEsvWriteReadResponse));
  BOOST_REQUIRE_EQUAL(uecho_message_iswriterequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyrequest(msg), false);
  BOOST_REQUIRE_EQUAL(uecho_message_iswriteresponse(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isreadresponse(msg), true);
  BOOST_REQUIRE_EQUAL(uecho_message_isnotifyresponse(msg), false);

  uecho_message_delete(msg);
}

BOOST_AUTO_TEST_CASE(MessageSearch)
{
  uEchoMessage* msg = uecho_message_search_new();

  BOOST_REQUIRE(uecho_message_issearchrequest(msg));
  BOOST_REQUIRE_EQUAL(uecho_message_size(msg), 14);

  uecho_message_delete(msg);
}
