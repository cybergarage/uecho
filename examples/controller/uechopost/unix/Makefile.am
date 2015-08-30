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

noinst_PROGRAMS = uechopost 

uechopost_SOURCES = \
	../uechopost.c

#if HAVE_LIBTOOL
#uechopost_LDADD = ../../../../lib/unix/libuecho.la
#else
uechopost_LDADD = ../../../../lib/unix/libuecho.a
#endif
