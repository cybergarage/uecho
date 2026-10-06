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

# ESP-IDF evaluates components before its top-level project() call. Register
# the library as an IDF component and skip the standalone host build.
if(ESP_PLATFORM)
  # The requirements pass runs this file in CMake script mode, where
  # CONFIGURE_DEPENDS is not allowed and sdkconfig values are not loaded.
  file(GLOB_RECURSE UECHO_SRCS "${CMAKE_CURRENT_LIST_DIR}/src/uecho/*.c")
  idf_component_register(
    SRCS ${UECHO_SRCS}
    INCLUDE_DIRS include
    PRIV_INCLUDE_DIRS src
    REQUIRES esp_netif lwip pthread)
  if(CMAKE_BUILD_EARLY_EXPANSION)
    return()
  endif()
  foreach(required_config CONFIG_LWIP_IPV4 CONFIG_LWIP_SO_REUSE)
    if(NOT ${required_config})
      message(FATAL_ERROR "uEcho requires ${required_config}=y; see doc/espidf.md")
    endif()
  endforeach()
  if(NOT CONFIG_LWIP_SO_REUSE_RXTOALL)
    message(WARNING "Enable CONFIG_LWIP_SO_REUSE_RXTOALL so that multicast reaches every uEcho node; see doc/espidf.md")
  endif()
  return()
endif()

project(uecho VERSION 1.4.0 LANGUAGES C CXX)

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
