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
	../MessageTest.cpp \
	../NodeListTest.cpp \
	../ObserverTest.cpp \
	../NodeTest.cpp \
	../TestDevice.cpp \
	../PropertyListTest.cpp \
	../ThreadTest.cpp \
	../PropertyTest.cpp \
	../ClassTest.cpp \
	../MutexTest.cpp \
	../ClassListTest.cpp \
	../ControllerTest.cpp \
	../SocketTest.cpp \
	../InterfaceTest.cpp \
	../ObjectListTest.cpp \
	../LogTest.cpp \
	../DeviceTest.cpp \
	../uEchoTest.cpp \
	../MiscTest.cpp \
	../ProfileTest.cpp \
	../ObjectTest.cpp \
	../ServerTest.cpp

#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
