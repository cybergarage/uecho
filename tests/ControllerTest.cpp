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

#include <uecho/controller_internal.h>
#include <uecho/util/timer.h>

#include "TestDevice.h"

BOOST_AUTO_TEST_CASE(ControllerRun)
{
  uEchoController *ctrl = uecho_controller_new();
  
  BOOST_CHECK(!uecho_controller_getuserdata(ctrl));
  
  BOOST_CHECK(uecho_controller_start(ctrl));
  BOOST_CHECK(uecho_controller_stop(ctrl));

  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerTID)
{
  uEchoController *ctrl = uecho_controller_new();
  
  uEchoTID firstTid = uecho_controller_getnexttid(ctrl);
  BOOST_CHECK(uEchoTidMin <= firstTid);
  BOOST_CHECK(firstTid <= uEchoTidMax);
  
  uEchoTID prevTid = firstTid;
  for (int n=0; n<100; n++) {
    uEchoTID tid = uecho_controller_getnexttid(ctrl);
    BOOST_CHECK(uEchoTidMin <= tid);
    BOOST_CHECK(prevTid < tid);
    BOOST_CHECK(tid <= uEchoTidMax);
  }
  
  uecho_controller_delete(ctrl);
}

BOOST_AUTO_TEST_CASE(ControllerOption)
{
  uEchoController *ctrl = uecho_controller_new();
  
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
  uEchoController *ctrl = uecho_controller_new();
  
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

BOOST_AUTO_TEST_CASE(ControllerSearchAll)
{
  // Create Controller (Disable UDP Server)

  uEchoController *ctrl = uecho_controller_new();
  uecho_controller_disableudpserver(ctrl);
  BOOST_CHECK(uecho_controller_start(ctrl));
  BOOST_CHECK(uecho_controller_isrunning(ctrl));
  
  // Start Device
  
  uEchoNode *node = uecho_test_createtestnode();
  BOOST_CHECK(uecho_node_start(node));

  // Search (NotificationRequest instead of ReadRequest)

  BOOST_CHECK(uecho_controller_searchallobjectswithesv(ctrl, uEchoEsvNotificationRequest));
  
  // Find device

  uEchoObject *foundObj = uecho_controller_getobjectbycodewithwait(ctrl, UECHO_TEST_OBJECTCODE, UECHO_TEST_RESPONSE_WAIT_MAX_MTIME);
  BOOST_CHECK(foundObj);

  // Teminate
  
  BOOST_CHECK(uecho_controller_stop(ctrl));
  uecho_controller_delete(ctrl);

  BOOST_CHECK(uecho_node_stop(node));
  uecho_node_delete(node);
}
