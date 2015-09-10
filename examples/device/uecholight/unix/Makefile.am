##################################################################
#
# uEcho for C
#
# Copyright (C) Satoshi Konno 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/include -I../ -I../../common

noinst_PROGRAMS = uecholight 

uecholight_SOURCES = \
	../main.c \
	../lighting_dev.c

#if HAVE_LIBTOOL
#uecholight_LDADD = ../../../../lib/unix/libuecho.la
#else
uecholight_LDADD = ../../../../lib/unix/libuecho.a
#endif
