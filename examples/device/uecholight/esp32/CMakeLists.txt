# CMakeLists.txt for uecholight ESP32 example

cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS ../../../../lib/esp32)

project(uecholight_example)

idf_component_register(SRCS "app_main.c" "main.c" "lighting_dev.c"
                    INCLUDE_DIRS ".")

# Add the uecho component dependency
# This assumes the uecho component is in the components directory
# or in EXTRA_COMPONENT_DIRS
# REQUIRES uecho
