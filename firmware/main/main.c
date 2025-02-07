#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include "wifi_api.h"
#include "rest_api.h"
#include "motor_api.h"

// Private config should include the defenitions:
// WIFI_SSID
// WIFI_PASSWORD
// SERVER_IP
// SERVER_PORT
#include "private_config.h" 
#include "io_config.h"
#include "spi_master.h"

#define TAG "main"

void app_main(void) {

    gpio_config(&io_config);

    init_wifi();
    esp_err_t ret = connect_wifi(WIFI_SSID, WIFI_PASSWORD);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        /*return;*/
        ESP_LOGI(TAG, "Continuing to main loop wihtout WiFi connection");
    }

    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA"), &ip_info);
    ESP_LOGI(TAG, "Server running on address: " IPSTR, IP2STR(&ip_info.ip));

    // Init REST API
    httpd_handle_t server_handle = start_webserver();

    ret = init_motor_config();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initalize motor config");
        return;
    }

    spi_device_handle_t LSM6DS3_handle = init_spi();
    ret = test_LSM6DS3_connection(LSM6DS3_handle);
    
    int running = 1;
    while(running) {
        vTaskDelay(pdMS_TO_TICKS(10));
        test_LSM6DS3_connection(LSM6DS3_handle);
    }

    stop_webserver(server_handle);
    deinit_wifi();
}

