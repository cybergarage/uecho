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

#include <uecho/_controller.h>
#include <uecho/util/timer.h>

#include "TestDevice.h"

BOOST_AUTO_TEST_CASE(ControllerRun)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_CHECK(!uecho_controller_getuserdata(ctrl));

  BOOST_CHECK(uecho_controller_start(ctrl));
  BOOST_CHECK(uecho_controller_stop(ctrl));

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerTID)
{
  uEchoController* ctrl = uecho_controller_new();

  uEchoTID first_tid = uecho_controller_getnexttid(ctrl);
  BOOST_CHECK(uEchoTidMin <= first_tid);
  BOOST_CHECK(first_tid <= uEchoTidMax);

  uEchoTID prev_tid = first_tid;
  for (int n = 0; n < 100; n++) {
    uEchoTID tid = uecho_controller_getnexttid(ctrl);
    BOOST_CHECK(uEchoTidMin <= tid);
    BOOST_CHECK(prev_tid < tid);
    BOOST_CHECK(tid <= uEchoTidMax);
  }

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerOption)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_CHECK_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoOptionNone), false);
  BOOST_CHECK_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), false);

  uecho_controller_enableoption(ctrl, uEchoControllerOptionDisableUdpServer);
  BOOST_CHECK_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), true);

  uecho_controller_disableoption(ctrl, uEchoControllerOptionDisableUdpServer);
  BOOST_CHECK_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), false);

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerUdpOption)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_CHECK_EQUAL(uecho_controller_isudpserverenabled(ctrl), true);
  BOOST_CHECK_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), true);

  uecho_controller_disableudpserver(ctrl);
  BOOST_CHECK_EQUAL(uecho_controller_isudpserverenabled(ctrl), false);
  BOOST_CHECK_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), false);

  uecho_controller_enableudpserver(ctrl);
  BOOST_CHECK_EQUAL(uecho_controller_isudpserverenabled(ctrl), true);
  BOOST_CHECK_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), true);

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerRequest)
{
  // Create Controller (Disable UDP Server)

  uEchoController* ctrl = uecho_controller_new();
  BOOST_CHECK(uecho_controller_start(ctrl));
  BOOST_CHECK(uecho_controller_isrunning(ctrl));

  // Add Device

  uEchoNode* node = uecho_controller_getlocalnode(ctrl);
  uEchoObject* dev = uecho_test_createtestdevice();
  BOOST_CHECK(uecho_node_addobject(node, dev));

  // Search (NotificationRequest instead of ReadRequest)

  BOOST_CHECK(uecho_controller_searchwithesv(ctrl, uEchoEsvNotificationRequest));

  // Find device

  uEchoObject* found_obj = uecho_controller_getobjectbycodewithwait(ctrl, UECHO_TEST_OBJECTCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_CHECK(found_obj);
  if (!found_obj)
    return;

  // Post Message (ReadRequest)

  uEchoMessage* msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 0, NULL));

  uEchoMessage* res = uecho_message_new();

  BOOST_CHECK(uecho_controller_postmessage(ctrl, found_obj, msg, res));

  BOOST_CHECK_EQUAL(uecho_message_getopc(res), 1);
  BOOST_CHECK_EQUAL(uecho_message_getesv(res), uEchoEsvReadResponse);
  uEchoProperty* prop = uecho_message_getproperty(res, 0);
  BOOST_CHECK(prop);
  if (!prop)
    return;
  BOOST_CHECK_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 1);
  byte* prop_data = uecho_property_getdata(prop);
  BOOST_CHECK(prop_data);
  BOOST_CHECK_EQUAL(prop_data[0], UECHO_TEST_PROPERTY_SWITCH_DEFAULT);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Post Write Message (WriteRequest:OFF)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteRequestResponseRequired);
  byte post_byte = UECHO_TEST_PROPERTY_SWITCH_OFF;
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, &post_byte, 1));

  res = uecho_message_new();

  BOOST_CHECK(uecho_controller_postmessage(ctrl, found_obj, msg, res));

  BOOST_CHECK_EQUAL(uecho_message_getopc(res), 1);
  BOOST_CHECK_EQUAL(uecho_message_getesv(res), uEchoEsvWriteResponse);
  prop = uecho_message_getproperty(res, 0);
  BOOST_CHECK(prop);
  if (!prop)
    return;
  BOOST_CHECK_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 0);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Post Message (ReadRequest)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  BOOST_CHECK(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 0, NULL));

  res = uecho_message_new();

  BOOST_CHECK(uecho_controller_postmessage(ctrl, found_obj, msg, res));

  BOOST_CHECK_EQUAL(uecho_message_getopc(res), 1);
  BOOST_CHECK_EQUAL(uecho_message_getesv(res), uEchoEsvReadResponse);
  prop = uecho_message_getproperty(res, 0);
  BOOST_CHECK(prop);
  if (!prop)
    return;
  BOOST_CHECK_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_CHECK_EQUAL(uecho_property_getdatasize(prop), 1);
  prop_data = uecho_property_getdata(prop);
  BOOST_CHECK(prop_data);
  BOOST_CHECK_EQUAL(prop_data[0], UECHO_TEST_PROPERTY_SWITCH_OFF);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Teminate

  BOOST_CHECK(uecho_controller_stop(ctrl));
  uecho_controller_delete(ctrl);
}
