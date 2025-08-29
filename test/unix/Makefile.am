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
	../BytesTest.cpp \
	../PropertyTest.cpp \
	../ThreadTest.cpp \
	../TestDevice.cpp \
	../ObserverTest.cpp \
	../PropertyMapTest.cpp \
	../InterfaceTest.cpp \
	../uEchoTest.cpp \
	../NodeListTest.cpp \
	../NodeTest.cpp \
	../ObjectTest.cpp \
	../MutexTest.cpp \
	../ControllerTest.cpp \
	../MiscTest.cpp \
	../ServerTest.cpp \
	../ClassListTest.cpp \
	../ProfileTest.cpp \
	../MessageTest.cpp \
	../ClassTest.cpp \
	../ObjectListTest.cpp \
	../LogTest.cpp \
	../DeviceTest.cpp \
	../SocketTest.cpp \
	../DatabaseTest.cpp \
	../PropertyListTest.cpp

uechotest_LDADD = ../../lib/unix/libuecho.a
