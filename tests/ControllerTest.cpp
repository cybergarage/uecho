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

BOOST_AUTO_TEST_CASE(ControllerRun)
{
  uEchoController *ctrl = uecho_controller_new();
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
