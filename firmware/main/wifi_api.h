#pragma once

#include <nvs_flash.h>
#include <esp_log.h>
#include <esp_err.h>

#include <esp_netif.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>

// @brief initalize the wifi module on the esp
esp_err_t init_wifi(void);

// @brief deinialize the wifi module on the esp
esp_err_t deinit_wifi(void);

// @brief connect to a wifi network with ssid and password
esp_err_t connect_wifi(char *wifi_ssid, char *password);

// @brief disconnect from wifi network
esp_err_t disconnect_wifi(void);
