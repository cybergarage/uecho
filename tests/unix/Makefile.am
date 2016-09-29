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
	../ThreadTest.cpp \
	../ClassListTest.cpp \
	../ControllerTest.cpp \
	../uEchoTest.cpp \
	../MutexTest.cpp \
	../MiscTest.cpp \
	../PropertyTest.cpp \
	../PropertyListTest.cpp \
	../ObserverTest.cpp \
	../ProfileTest.cpp \
	../ServerTest.cpp \
	../ObjectTest.cpp \
	../NodeTest.cpp \
	../NodeListTest.cpp \
	../TestDevice.cpp \
	../ClassTest.cpp \
	../InterfaceTest.cpp \
	../MessageTest.cpp \
	../SocketTest.cpp \
	../DeviceTest.cpp \
	../ObjectListTest.cpp
#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
