##################################################################
#
# uEcho for C
#
# Copyright (C) The uecho Authors 2015
#
# This is licensed under BSD-style license, see file COPYING.
#
##################################################################

cmake_minimum_required(VERSION 3.10)

project(uecho VERSION 1.2.2 LANGUAGES C CXX)

option(UECHO_ENABLE_TEST     "Build tests"    OFF)
option(UECHO_ENABLE_EXAMPLES "Build examples" ON)

add_subdirectory(lib/unix)

if(UECHO_ENABLE_TEST)
  enable_testing()
  add_subdirectory(test/unix)
endif()

if(UECHO_ENABLE_EXAMPLES)
  add_subdirectory(examples/controller/uechosearch)
  add_subdirectory(examples/controller/uechodump)
  add_subdirectory(examples/controller/uechopost)
  add_subdirectory(examples/controller/uechobench)
  add_subdirectory(examples/device/uecholight)
endif()
