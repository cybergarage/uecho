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

#include <uecho/controller.h>
#include <uecho/util/timer.h>

#include "TestDevice.h"

const int UECHO_TEST_SEARCH_WAIT_MAX_MTIME = 10000;
const int UECHO_TEST_SEARCH_WAIT_RETLY_CNT = 100;

BOOST_AUTO_TEST_CASE(ControllerRun)
{
  uEchoController *cp = uecho_controller_new();
  
  BOOST_CHECK(uecho_controller_start(cp));
  BOOST_CHECK(uecho_controller_stop(cp));

  uecho_controller_delete(cp);
}

BOOST_AUTO_TEST_CASE(ControllerTID)
{
  uEchoController *cp = uecho_controller_new();
  
  uEchoTID firstTid = uecho_controller_getnexttid(cp);
  BOOST_CHECK(uEchoTidMin <= firstTid);
  BOOST_CHECK(firstTid <= uEchoTidMax);
  
  uEchoTID prevTid = firstTid;
  for (int n=0; n<100; n++) {
    uEchoTID tid = uecho_controller_getnexttid(cp);
    BOOST_CHECK(uEchoTidMin <= tid);
    BOOST_CHECK(prevTid < tid);
    BOOST_CHECK(tid <= uEchoTidMax);
  }
  
  uecho_controller_delete(cp);
}

BOOST_AUTO_TEST_CASE(ControllerSearchAll)
{
  // Start Device

  uEchoNode *node = uecho_test_createtestnode();
  BOOST_CHECK(uecho_node_start(node));

  // Start Controller

  uEchoController *cp = uecho_controller_new();
  BOOST_CHECK(uecho_controller_start(cp));

  // Find device

  BOOST_CHECK(uecho_controller_searchallobjects(cp));
  
  uEchoObject *foundObj;
  for (int n=0; n<UECHO_TEST_SEARCH_WAIT_RETLY_CNT; n++) {
    uecho_sleep(UECHO_TEST_SEARCH_WAIT_MAX_MTIME / UECHO_TEST_SEARCH_WAIT_RETLY_CNT);
    foundObj = uecho_controller_getobjectbycode(cp, UECHO_TEST_OBJECTCODE);
    if (foundObj)
      break;
  }

  BOOST_CHECK(foundObj);
  
  // Send Message
  
  uEchoMessage *msg = uecho_message_new();
  uecho_message_setesv(msg, uEchoEsvReadRequest);
  uecho_message_setproperty(msg, UECHO_TEST_PROPERTY_SWITCHCODE, 0, NULL);
  BOOST_CHECK(uecho_controller_sendmessage(cp, foundObj, msg));
  uecho_message_delete(msg);

  // Teminate
  
  BOOST_CHECK(uecho_controller_stop(cp));
  uecho_controller_delete(cp);

  BOOST_CHECK(uecho_node_stop(node));
  uecho_node_delete(node);
}
