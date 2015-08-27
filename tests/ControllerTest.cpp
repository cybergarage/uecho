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

#include "TestDevice.h"

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


BOOST_AUTO_TEST_CASE(ControllerSearch)
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
