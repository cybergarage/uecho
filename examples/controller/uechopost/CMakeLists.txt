##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

add_executable(uechopost uechopost.c)

target_include_directories(uechopost PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(uechopost PRIVATE uecho)
