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
	../TestDevice.h

uechotest_SOURCES = \
	../NodeListTest.cpp \
	../ProfileTest.cpp \
	../InterfaceTest.cpp \
	../PropertyListTest.cpp \
	../PropertyTest.cpp \
	../uEchoTest.cpp \
	../ObserverTest.cpp \
	../ServerTest.cpp \
	../ClassTest.cpp \
	../ThreadTest.cpp \
	../TestDevice.cpp \
	../NodeTest.cpp \
	../ControllerTest.cpp \
	../ObjectTest.cpp \
	../MiscTest.cpp \
	../MessageTest.cpp \
	../ObjectListTest.cpp \
	../MutexTest.cpp \
	../ClassListTest.cpp \
	../DeviceTest.cpp \
	../SocketTest.cpp

#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
