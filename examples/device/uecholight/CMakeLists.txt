##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

add_executable(uecholight
  main.c
  lighting_dev.c
)

target_include_directories(uecholight PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(uecholight PRIVATE uecho)
