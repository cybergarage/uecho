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

noinst_PROGRAMS = uechobench 

uechobench_SOURCES = \
	../uechobench.c

#if HAVE_LIBTOOL
#uechobench_LDADD = ../../../../lib/unix/libuecho.la
#else
uechobench_LDADD = ../../../../lib/unix/libuecho.a
#endif
