##################################################################
#
# uEcho for C
#
# Copyright (C) Satoshi Konno 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################
#if HAVE_LIBTOOL
#lib_LTLIBRARIES = libuecho.la
#else
lib_LIBRARIES = libuecho.a
#endif

AM_CFLAGS = -I$(top_builddir)/include -I$(top_builddir)/src

uecho_sources = \
	../../src/uecho/controller.c \
	../../src/uecho/net/interface_function.c \
	../../src/uecho/net/interface.c \
	../../src/uecho/net/datagram_packet.c \
	../../src/uecho/net/interface_list.c \
	../../src/uecho/net/net_function.c \
	../../src/uecho/net/socket.c \
	../../src/uecho/net/socket_opt.c \
	../../src/uecho/object.c \
	../../src/uecho/class.c \
	../../src/uecho/property_enum_list.c \
	../../src/uecho/util/log.c \
	../../src/uecho/util/thread.c \
	../../src/uecho/util/list.c \
	../../src/uecho/util/timer.c \
	../../src/uecho/util/strings.c \
	../../src/uecho/util/bytes.c \
	../../src/uecho/util/thread_list.c \
	../../src/uecho/util/mutex.c \
	../../src/uecho/util/cond.c \
	../../src/uecho/util/strings_function.c \
	../../src/uecho/util/strings_tokenizer.c \
	../../src/uecho/util/logs.c \
	../../src/uecho/property_list.c \
	../../src/uecho/node_list.c \
	../../src/uecho/manufacture_list.c \
	../../src/uecho/property.c \
	../../src/uecho/manufacture.c \
	../../src/uecho/std/manufacturers.c \
	../../src/uecho/std/standard.c \
	../../src/uecho/std/database.c \
	../../src/uecho/std/profile_super_class.c \
	../../src/uecho/std/node_profile_class.c \
	../../src/uecho/std/objects_mra.c \
	../../src/uecho/std/object_super_class.c \
	../../src/uecho/std/profile.c \
	../../src/uecho/std/device.c \
	../../src/uecho/std/device_super_class.c \
	../../src/uecho/frame/object_property_observer.c \
	../../src/uecho/frame/message_observer_manager.c \
	../../src/uecho/frame/mcast_server.c \
	../../src/uecho/frame/udp_server_list.c \
	../../src/uecho/frame/udp_server.c \
	../../src/uecho/frame/server.c \
	../../src/uecho/frame/mcast_server_list.c \
	../../src/uecho/frame/message_observer.c \
	../../src/uecho/frame/message_observer_list.c \
	../../src/uecho/frame/object_property_observer_list.c \
	../../src/uecho/frame/object_property_observer_manager.c \
	../../src/uecho/class_list.c \
	../../src/uecho/property_enum.c \
	../../src/uecho/node_listener.c \
	../../src/uecho/controller_listener.c \
	../../src/uecho/esv.c \
	../../src/uecho/object_list.c \
	../../src/uecho/misc.c \
	../../src/uecho/message_search.c \
	../../src/uecho/node.c \
	../../src/uecho/message.c

libuechoincludedir = $(includedir)/uecho
nobase_libuechoinclude_HEADERS =  \
        $(uecho_headers)

#if HAVE_LIBTOOL
#libuecho_la_SOURCES = \
#        $(uecho_source)
#libuecho_la_CFLAGS = $(AM_CFLAGS)
#else
libuecho_a_SOURCES = \
        $(uecho_sources)
libuecho_a_CFLAGS = $(AM_CFLAGS)
#endif
