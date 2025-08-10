##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

SUBDIRS = include lib

if UECHO_ENABLE_EXAMPLES
SUBDIRS += examples
endif

if UECHO_ENABLE_TEST
SUBDIRS += test
endif

if UECHO_BUILD_ESP32
SUBDIRS += lib/esp32
endif
