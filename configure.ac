#############################################################################
#
# eEcho for C
# 
# Copyright (C) The uecho Authors 2015
#
#############################################################################

AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(config.h)

AC_INIT([echo], [0.8])
AC_CONFIG_SRCDIR([include/mupnp/typedef.h])
AM_INIT_AUTOMAKE([subdir-objects])

##############################
# Constants
##############################

#CppUnit version
CPPUNIT_VERSION=1.6.0

##############################
# Checks for programs.
##############################

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_AWK
#AC_PROG_LIBTOOL
AC_PROG_RANLIB

##############################
# Checks for Network functions
##############################

AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([inet_ntoa])
AC_CHECK_HEADERS([ifaddrs.h])
AC_CHECK_FUNCS([getifaddrs])

##### socklen_t ####
AC_MSG_CHECKING(for socklen_t)
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
socklen_t foosocket;
],
[],
[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SOCKLEN_T],1,[SOCKLEN_T])],
[AC_MSG_RESULT(no)]
)

##### SIOCGIFHWADDR ####
AC_MSG_CHECKING(for SIOCGIFHWADDR)
AC_TRY_COMPILE([
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
void func()
{
  int sock;
  struct ifreq ifr;
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
  ifr.ifr_addr.sa_family = AF_INET;
  ioctl(sock, SIOCGIFHWADDR, &ifr);
}
],
[],
[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SIOCGIFHWADDR],1,[SIOCGIFHWADDR])],
[AC_MSG_RESULT(no)]
)

##### sockaddr_dl ####
AC_MSG_CHECKING(for sockaddr_dl)
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
void func()
{
  struct ifaddrs *ifaddr;
  struct ifaddrs *i;
  struct sockaddr_dl *dl;
  unsigned char *macaddr;
  getifaddrs(&ifaddr);
  dl = (struct sockaddr_dl *)(ifaddr->ifa_addr);
  LLADDR(dl);
  freeifaddrs(ifaddr);
}
],
[],
[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SOCKADDR_DL],1,[SOCKADDR_DL])],
[AC_MSG_RESULT(no)]
)

##### IP_PKTINFO ####
AC_MSG_CHECKING(for IP_PKTINFO)
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
void func()
{
  int on=1;
  setsockopt(0, IPPROTO_IP, IP_PKTINFO,  &on, sizeof(on));
}
],
[],
[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_IP_PKTINFO],1,[IP_PKTINFO])],
[AC_MSG_RESULT(no)]
)

##### SO_NOSIGPIPE ####
AC_MSG_CHECKING(for SO_NOSIGPIPE)
AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/socket.h>
void func()
{
  int on=1;
  setsockopt(0, SOL_SOCKET, SO_NOSIGPIPE,  &on, sizeof(on));
}
],
[],
[AC_MSG_RESULT(yes)
AC_DEFINE([HAVE_SO_NOSIGPIPE],1,[SO_NOSIGPIPES])],
[AC_MSG_RESULT(no)]
)

##############################
# Testing
##############################

AC_ARG_ENABLE([tests], AC_HELP_STRING([--enable-tests], [ build tests (default = no) ]), [build_tests="yes"], [])
AM_CONDITIONAL(UNITTESTS,test "$build_tests" = yes)
if [ test "$build_tests" = yes ]; then
        AC_CHECK_LIB([boost_unit_test_framework-mt],[main],,[AC_MSG_ERROR(uHTTP unittests need boost::unit_test_framework)])
fi

##############################
# Makefiles
##############################

AC_OUTPUT([
clinkc.pc
Makefile
include/Makefile
lib/Makefile
lib/unix/Makefile
tests/Makefile
tests/unix/Makefile
])
