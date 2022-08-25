##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

TESTS = uechotest

check_PROGRAMS = uechotest

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src -I../
	../TestDevice.h

uechotest_SOURCES = \
	../MiscTest.cpp \
	../PropertyTest.cpp \
	../BytesTest.cpp \
	../InterfaceTest.cpp \
	../ObserverTest.cpp \
	../LogTest.cpp \
	../MessageTest.cpp \
	../ObjectTest.cpp \
	../ClassListTest.cpp \
	../SocketTest.cpp \
	../ProfileTest.cpp \
	../ServerTest.cpp \
	../TestDevice.cpp \
	../ClassTest.cpp \
	../ControllerTest.cpp \
	../NodeTest.cpp \
	../uEchoTest.cpp \
	../MutexTest.cpp \
	../ObjectListTest.cpp \
	../DatabaseTest.cpp \
	../NodeListTest.cpp \
	../DeviceTest.cpp \
	../ThreadTest.cpp \
	../PropertyListTest.cpp

#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
