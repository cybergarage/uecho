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
	../MutexTest.cpp \
	../ThreadTest.cpp \
	../ClassListTest.cpp \
	../DeviceTest.cpp \
	../TestDevice.cpp \
	../LogTest.cpp \
	../uEchoTest.cpp \
	../ObjectTest.cpp \
	../PropertyListTest.cpp \
	../ControllerTest.cpp \
	../MiscTest.cpp \
	../PropertyTest.cpp \
	../InterfaceTest.cpp \
	../NodeListTest.cpp \
	../SocketTest.cpp \
	../ServerTest.cpp \
	../ClassTest.cpp \
	../NodeTest.cpp \
	../MessageTest.cpp \
	../ProfileTest.cpp \
	../ObserverTest.cpp \
	../ObjectListTest.cpp

#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
