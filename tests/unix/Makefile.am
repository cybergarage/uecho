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

	..//TestDevice.h

uechotest_SOURCES = \
	..//ClassListTest.cpp \
	..//ClassTest.cpp \
	..//ControllerTest.cpp \
	..//DeviceTest.cpp \
	..//InterfaceTest.cpp \
	..//MessageTest.cpp \
	..//MiscTest.cpp \
	..//MutexTest.cpp \
	..//NodeListTest.cpp \
	..//NodeTest.cpp \
	..//ObjectListTest.cpp \
	..//ObjectTest.cpp \
	..//ObserverTest.cpp \
	..//ProfileTest.cpp \
	..//PropertyListTest.cpp \
	..//PropertyTest.cpp \
	..//ServerTest.cpp \
	..//SocketTest.cpp \
	..//TestDevice.cpp \
	..//ThreadTest.cpp \
	..//uEchoTest.cpp
#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
