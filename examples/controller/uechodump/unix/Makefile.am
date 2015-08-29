##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/include -I../ -I../../common

noinst_PROGRAMS = uechodump 

uechodump_SOURCES = \
	../uechodump.c

#if HAVE_LIBTOOL
#uechodump_LDADD = ../../../../lib/unix/libuecho.la
#else
uechodump_LDADD = ../../../../lib/unix/libuecho.a
#endif
