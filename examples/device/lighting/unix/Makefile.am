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

noinst_PROGRAMS = uecho_lighting_dev 

uecho_lighting_dev_SOURCES = \
	../main.c \
	../lighting_dev.c

#if HAVE_LIBTOOL
#uecho_lighting_dev_LDADD = ../../../../lib/unix/libuecho.la
#else
uecho_lighting_dev_LDADD = ../../../../lib/unix/libuecho.a
#endif
