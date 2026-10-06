/******************************************************************
 *
 * uEcho for C
 *
 * Copyright (C) The uecho Authors 2015
 *
 * This is licensed under BSD-style license, see file COPYING.
 *
 ******************************************************************/

/*
 * ESP-IDF example: an ECHONET Lite mono functional lighting device.
 *
 * The board joins the Wi-Fi network configured in menuconfig, then starts a
 * uEcho node with a lighting object (0x029101). Writes to the operation
 * status property (EPC 0x80) switch an optional LED GPIO.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "driver/gpio.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "nvs_flash.h"

#include <uecho/device.h>
#include <uecho/node.h>

// See : APPENDIX Detailed Requirements for ECHONET Device objects
//       3.3.29 Requirements for mono functional lighting class

#define LIGHT_OBJECT_CODE 0x029101
#define LIGHT_PROPERTY_POWER_CODE 0x80
#define LIGHT_PROPERTY_POWER_ON 0x30
#define LIGHT_PROPERTY_POWER_OFF 0x31

#define WIFI_GOT_IP_BIT BIT0
#define WIFI_LOST_IP_BIT BIT1

static const char* TAG = "uecholight";
static EventGroupHandle_t wifiEventGroup;

/****************************************
 * LED
 ****************************************/

static void uecho_light_setled(bool on)
{
#if CONFIG_UECHO_EXAMPLE_LED_GPIO >= 0
  gpio_set_level((gpio_num_t)CONFIG_UECHO_EXAMPLE_LED_GPIO, on ? 1 : 0);
#endif
  ESP_LOGI(TAG, "POWER = %s", on ? "ON" : "OFF");
}

static void uecho_light_initled(void)
{
#if CONFIG_UECHO_EXAMPLE_LED_GPIO >= 0
  gpio_reset_pin((gpio_num_t)CONFIG_UECHO_EXAMPLE_LED_GPIO);
  gpio_set_direction((gpio_num_t)CONFIG_UECHO_EXAMPLE_LED_GPIO, GPIO_MODE_OUTPUT);
#endif
}

/****************************************
 * ECHONET Lite lighting object
 ****************************************/

/* Called on a uEcho worker thread; returning true makes uEcho store the new value. */
static bool uecho_light_powerrequesthandler(uEchoObject* obj, uEchoProperty* prop, uEchoEsv esv, size_t pdc, byte* edt)
{
  if ((pdc != 1) || !edt)
    return false;

  switch (edt[0]) {
  case LIGHT_PROPERTY_POWER_ON:
    uecho_light_setled(true);
    return true;
  case LIGHT_PROPERTY_POWER_OFF:
    uecho_light_setled(false);
    return true;
  default:
    ESP_LOGW(TAG, "Rejected operation status 0x%02X", edt[0]);
    return false;
  }
}

static uEchoObject* uecho_light_new(void)
{
  uEchoObject* obj;
  byte status;

  obj = uecho_device_new();
  if (!obj)
    return NULL;

  uecho_object_setmanufacturercode(obj, CONFIG_UECHO_EXAMPLE_MANUFACTURER_CODE);
  uecho_object_setcode(obj, LIGHT_OBJECT_CODE);

  uecho_object_setproperty(obj, LIGHT_PROPERTY_POWER_CODE, uEchoPropertyAttrReadWrite);
  status = LIGHT_PROPERTY_POWER_OFF;
  uecho_object_setpropertydata(obj, LIGHT_PROPERTY_POWER_CODE, &status, 1);
  uecho_object_setpropertywriterequesthandler(obj, LIGHT_PROPERTY_POWER_CODE, uecho_light_powerrequesthandler);

  return obj;
}

/****************************************
 * Wi-Fi
 ****************************************/

static void uecho_wifi_eventhandler(void* arg, esp_event_base_t base, int32_t id, void* data)
{
  if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  }
  else if (base == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGW(TAG, "Wi-Fi disconnected, reconnecting");
    xEventGroupSetBits(wifiEventGroup, WIFI_LOST_IP_BIT);
    esp_wifi_connect();
  }
  else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)data;
    ESP_LOGI(TAG, "Got IP " IPSTR, IP2STR(&event->ip_info.ip));
    xEventGroupSetBits(wifiEventGroup, WIFI_GOT_IP_BIT);
  }
}

static void uecho_wifi_start(void)
{
  wifi_init_config_t initConfig = WIFI_INIT_CONFIG_DEFAULT();
  wifi_config_t wifiConfig = { 0 };

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  esp_netif_create_default_wifi_sta();
  ESP_ERROR_CHECK(esp_wifi_init(&initConfig));

  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, uecho_wifi_eventhandler, NULL, NULL));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, uecho_wifi_eventhandler, NULL, NULL));

  strlcpy((char*)wifiConfig.sta.ssid, CONFIG_UECHO_EXAMPLE_WIFI_SSID, sizeof(wifiConfig.sta.ssid));
  strlcpy((char*)wifiConfig.sta.password, CONFIG_UECHO_EXAMPLE_WIFI_PASSWORD, sizeof(wifiConfig.sta.password));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifiConfig));
  /* Modem sleep delays multicast delivery; keep the radio awake for ECHONET Lite discovery. */
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
  ESP_ERROR_CHECK(esp_wifi_start());
}

/****************************************
 * app_main
 ****************************************/

void app_main(void)
{
  esp_err_t err;
  uEchoNode* node;
  uEchoObject* obj;
  EventBits_t bits;

  if (strlen(CONFIG_UECHO_EXAMPLE_WIFI_SSID) == 0) {
    ESP_LOGE(TAG, "Set the Wi-Fi SSID with 'idf.py menuconfig' (uEcho light example)");
    return;
  }

  err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  uecho_light_initled();
  uecho_light_setled(false);

  wifiEventGroup = xEventGroupCreate();
  uecho_wifi_start();

  node = uecho_node_new();
  obj = uecho_light_new();
  if (!node || !obj) {
    ESP_LOGE(TAG, "Out of memory");
    return;
  }
  uecho_node_addobject(node, obj);

  for (;;) {
    /* Start (or restart) the node every time an address is assigned so that its sockets bind to the new address. */
    xEventGroupWaitBits(wifiEventGroup, WIFI_GOT_IP_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
    xEventGroupClearBits(wifiEventGroup, WIFI_LOST_IP_BIT);

    if (uecho_node_isrunning(node))
      uecho_node_stop(node);
    if (uecho_node_start(node))
      ESP_LOGI(TAG, "uEcho node started");
    else
      ESP_LOGE(TAG, "uEcho node failed to start");

    bits = xEventGroupWaitBits(wifiEventGroup, WIFI_LOST_IP_BIT, pdTRUE, pdFALSE, portMAX_DELAY);
    if (bits & WIFI_LOST_IP_BIT) {
      ESP_LOGW(TAG, "Stopping uEcho node until Wi-Fi reconnects");
      uecho_node_stop(node);
    }
  }
}
