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
	../ThreadTest.cpp \
	../ObjectTest.cpp \
	../MessageTest.cpp \
	../ServerTest.cpp \
	../ProfileTest.cpp \
	../ObserverTest.cpp \
	../ControllerTest.cpp \
	../PropertyTest.cpp \
	../NodeTest.cpp \
	../ClassTest.cpp \
	../InterfaceTest.cpp \
	../DatabaseTest.cpp \
	../uEchoTest.cpp \
	../DeviceTest.cpp \
	../SocketTest.cpp \
	../BytesTest.cpp \
	../TestDevice.cpp \
	../LogTest.cpp \
	../ObjectListTest.cpp \
	../PropertyMapTest.cpp \
	../PropertyListTest.cpp \
	../MiscTest.cpp \
	../NodeListTest.cpp \
	../ClassListTest.cpp \
	../MutexTest.cpp

uechotest_LDADD = ../../lib/unix/libuecho.a
