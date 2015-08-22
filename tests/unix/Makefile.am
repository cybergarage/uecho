##################################################################
#
# uEcho for C
#
# Copyright (C) Satoshi Konno 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################
noinst_PROGRAMS = uechotest

AM_CPPFLAGS = -I$(top_srcdir)/include -I../



uechotest_SOURCES = \
	..//ClassListTest.cpp \
	..//ClassTest.cpp \
	..//ControllerTest.cpp \
	..//DeviceTest.cpp \
	..//MessageTest.cpp \
	..//MutexTest.cpp \
	..//NodeTest.cpp \
	..//ObjectListTest.cpp \
	..//ObjectTest.cpp \
	..//ProfileTest.cpp \
	..//PropertyListTest.cpp \
	..//PropertyTest.cpp \
	..//ThreadTest.cpp \
	..//uEchoTest.cpp
#if HAVE_LIBTOOL
#uechotest_LDADD = ../../lib/unix/libuecho.la
#else
uechotest_LDADD = ../../lib/unix/libuecho.a
#endif
