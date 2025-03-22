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

  BOOST_REQUIRE(!uecho_controller_getuserdata(ctrl));

  BOOST_REQUIRE(uecho_controller_start(ctrl));
  BOOST_REQUIRE(uecho_controller_stop(ctrl));

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerOption)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoOptionNone), false);
  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), false);

  uecho_controller_enableoption(ctrl, uEchoControllerOptionDisableUdpServer);
  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), true);

  uecho_controller_disableoption(ctrl, uEchoControllerOptionDisableUdpServer);
  BOOST_REQUIRE_EQUAL(uecho_controller_isoptionenabled(ctrl, uEchoControllerOptionDisableUdpServer), false);

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerUdpOption)
{
  uEchoController* ctrl = uecho_controller_new();

  BOOST_REQUIRE_EQUAL(uecho_controller_isudpserverenabled(ctrl), true);
  BOOST_REQUIRE_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), true);

  uecho_controller_disableudpserver(ctrl);
  BOOST_REQUIRE_EQUAL(uecho_controller_isudpserverenabled(ctrl), false);
  BOOST_REQUIRE_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), false);

  uecho_controller_enableudpserver(ctrl);
  BOOST_REQUIRE_EQUAL(uecho_controller_isudpserverenabled(ctrl), true);
  BOOST_REQUIRE_EQUAL(uecho_server_isudpserverenabled(ctrl->node->server), true);

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerRequest)
{
  // Create Controller (Disable UDP Server)

  uEchoController* ctrl = uecho_controller_new();
  BOOST_REQUIRE(uecho_controller_start(ctrl));
  BOOST_REQUIRE(uecho_controller_isrunning(ctrl));

  // Add Device

  uEchoNode* node = uecho_controller_getlocalnode(ctrl);
  uEchoObject* dev = uecho_test_createtestdevice();
  BOOST_REQUIRE(uecho_node_addobject(node, dev));

  // Search (NotificationRequest instead of ReadRequest)

  BOOST_REQUIRE(uecho_controller_searchwithesv(ctrl, uEchoEsvNotificationRequest));

  // Find device

  uEchoObject* foundObj = uecho_controller_getobjectbycodewithwait(ctrl, UECHO_TEST_OBJECTCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_REQUIRE(foundObj);
  if (!foundObj)
    return;

  uEchoNode* foundNode = uecho_object_getparentnode(foundObj);
  BOOST_REQUIRE(foundNode);
  if (!foundNode)
    return;

  // Post Message (ReadRequest)

  uEchoMessage* msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));

  uEchoMessage* res = uecho_message_new();

  BOOST_REQUIRE(uecho_controller_postmessage(ctrl, foundNode, msg, res));

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(res), 1);
  BOOST_REQUIRE_EQUAL(uecho_message_getesv(res), uEchoEsvReadResponse);
  uEchoProperty* prop = uecho_message_getproperty(res, 0);
  BOOST_REQUIRE(prop);
  if (!prop)
    return;
  BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 1);
  byte* propData = uecho_property_getdata(prop);
  BOOST_REQUIRE(propData);
  BOOST_REQUIRE_EQUAL(propData[0], UECHO_TEST_PROPERTY_SWITCH_DEFAULT);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Post Write Message (WriteRequest:OFF)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvWriteRequestResponseRequired);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  byte postByte = UECHO_TEST_PROPERTY_SWITCH_OFF;
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, &postByte, 1));

  res = uecho_message_new();

  BOOST_REQUIRE(uecho_controller_postmessage(ctrl, foundNode, msg, res));

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(res), 1);
  BOOST_REQUIRE_EQUAL(uecho_message_getesv(res), uEchoEsvWriteResponse);
  prop = uecho_message_getproperty(res, 0);
  BOOST_REQUIRE(prop);
  if (!prop)
    return;
  BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 0);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Post Message (ReadRequest)

  msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setdestinationobjectcode(msg, uecho_object_getcode(foundObj));
  BOOST_REQUIRE(uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, NULL, 0));

  res = uecho_message_new();

  BOOST_REQUIRE(uecho_controller_postmessage(ctrl, foundNode, msg, res));

  BOOST_REQUIRE_EQUAL(uecho_message_getopc(res), 1);
  BOOST_REQUIRE_EQUAL(uecho_message_getesv(res), uEchoEsvReadResponse);
  prop = uecho_message_getproperty(res, 0);
  BOOST_REQUIRE(prop);
  if (!prop)
    return;
  BOOST_REQUIRE_EQUAL(uecho_property_getcode(prop), UECHO_TEST_PROPERTY_SWITCHCODE);
  BOOST_REQUIRE_EQUAL(uecho_property_getdatasize(prop), 1);
  propData = uecho_property_getdata(prop);
  BOOST_REQUIRE(propData);
  BOOST_REQUIRE_EQUAL(propData[0], UECHO_TEST_PROPERTY_SWITCH_OFF);

  uecho_message_delete(res);
  uecho_message_delete(msg);

  // Teminate

  BOOST_REQUIRE(uecho_controller_stop(ctrl));
  uecho_controller_delete(ctrl);
}
