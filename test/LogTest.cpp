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

#include <array>
#include <string>
#include <uecho/net/interface.h>
#include <uecho/util/log.h>
#include <uecho/util/logs.h>

BOOST_AUTO_TEST_CASE(LogFunction)
{
  const char* msg = "hello";

  uecho_log_debug("%s", msg);
  uecho_log_warn("%s", msg);
  uecho_log_error("%s", msg);
  uecho_log_info("%s", msg);
}

BOOST_AUTO_TEST_CASE(LogOversizedPackets)
{
  std::array<byte, 512> packet;
  packet.fill(0xAB);
  std::string longText(1024, 'x');
  for (uEchoLogLevel level : { UECHO_LOG_NONE, UECHO_LOG_ALL }) {
    uecho_log_setlevel(level);
    for (size_t length : { size_t(1), size_t(109), size_t(110), size_t(111), packet.size() }) {
      uecho_net_socket_debug("R", UECHO_NET_IPV4_LOOPBACK, UECHO_NET_IPV4_LOOPBACK, packet.data(), length);
      uecho_net_socket_error(longText.c_str(), longText.c_str(), longText.c_str(), packet.data(), length);
    }
    uecho_net_socket_debug(nullptr, nullptr, nullptr, packet.data(), packet.size());
    uecho_net_socket_debug("R", "", "", nullptr, 1);
  }
  uecho_log_setlevel(UECHO_LOG_DEBUG);
}
