##################################################################
#
# uEcho for C
#
# Copyright (C) Satoshi Konno 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

TESTS = uechotest

check_PROGRAMS = uechotest

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src -I../
	..//TestDevice.h

uechotest_SOURCES = \
	..//ObjectTest.cpp \
	..//ObserverTest.cpp \
	..//BytesTest.cpp \
	..//uEchoTest.cpp \
	..//ServerTest.cpp \
	..//TestDevice.cpp \
	..//ObjectListTest.cpp \
	..//ThreadTest.cpp \
	..//NodeTest.cpp \
	..//PropertyTest.cpp \
	..//DatabaseTest.cpp \
	..//SocketTest.cpp \
	..//DeviceTest.cpp \
	..//ControllerTest.cpp \
	..//MessageTest.cpp \
	..//MutexTest.cpp \
	..//PropertyMapTest.cpp \
	..//LogTest.cpp \
	..//NodeListTest.cpp \
	..//MiscTest.cpp \
	..//InterfaceTest.cpp \
	..//PropertyListTest.cpp \
	..//ClassListTest.cpp \
	..//ClassTest.cpp \
	..//ProfileTest.cpp

#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
