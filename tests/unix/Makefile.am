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
	../ProfileTest.cpp \
	../ClassListTest.cpp \
	../NodeListTest.cpp \
	../ObjectListTest.cpp \
	../uEchoTest.cpp \
	../SocketTest.cpp \
	../ThreadTest.cpp \
	../ControllerTest.cpp \
	../ClassTest.cpp \
	../TestDevice.cpp \
	../ObserverTest.cpp \
	../PropertyListTest.cpp \
	../NodeTest.cpp \
	../MessageTest.cpp \
	../MutexTest.cpp \
	../InterfaceTest.cpp \
	../PropertyTest.cpp \
	../DeviceTest.cpp \
	../ObjectTest.cpp \
	../ServerTest.cpp \
	../MiscTest.cpp
#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
