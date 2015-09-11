##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################
noinst_PROGRAMS = uechotest

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src -I../

	../TestDevice.h

uechotest_SOURCES = \
	../ClassListTest.cpp \
	../TestDevice.cpp \
	../ProfileTest.cpp \
	../NodeTest.cpp \
	../ObjectListTest.cpp \
	../ThreadTest.cpp \
	../ObserverTest.cpp \
	../MiscTest.cpp \
	../MessageTest.cpp \
	../SocketTest.cpp \
	../ControllerTest.cpp \
	../uEchoTest.cpp \
	../NodeListTest.cpp \
	../ObjectTest.cpp \
	../InterfaceTest.cpp \
	../PropertyTest.cpp \
	../DeviceTest.cpp \
	../ClassTest.cpp \
	../ServerTest.cpp \
	../PropertyListTest.cpp \
	../MutexTest.cpp
#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
