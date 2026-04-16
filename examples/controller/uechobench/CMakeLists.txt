##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

add_executable(uechobench uechobench.c)

target_include_directories(uechobench PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(uechobench PRIVATE uecho)
