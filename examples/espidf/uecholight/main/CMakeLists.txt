# EXTRA_COMPONENT_DIRS uses the checkout directory's basename as its name.
get_filename_component(UECHO_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../../.." ABSOLUTE)
get_filename_component(UECHO_COMPONENT_NAME "${UECHO_ROOT}" NAME)
idf_component_register(SRCS "uecholight.c"
  INCLUDE_DIRS "."
  REQUIRES ${UECHO_COMPONENT_NAME} esp_driver_gpio esp_wifi esp_event esp_netif nvs_flash)
