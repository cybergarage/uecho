##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

add_executable(uechosearch uechosearch.c)

target_include_directories(uechosearch PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(uechosearch PRIVATE uecho)
