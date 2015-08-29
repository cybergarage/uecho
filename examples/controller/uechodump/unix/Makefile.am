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

noinst_PROGRAMS = uecho_search 

uecho_search_SOURCES = \
	../uecho_search.c

#if HAVE_LIBTOOL
#uecho_search_LDADD = ../../../../lib/unix/libuecho.la
#else
uecho_search_LDADD = ../../../../lib/unix/libuecho.a
#endif
