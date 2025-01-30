#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include "wifi_api.h"
#include "rest_api.h"

// Private config should include the defenitions:
// WIFI_SSID
// WIFI_PASSWORD
// SERVER_IP
// SERVER_PORT
#include "private_config.h" 

#define LED_PIN GPIO_NUM_14
#define TAG "main"

void app_main(void) {
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << LED_PIN), // 64 bit mask
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    }; 

    gpio_config(&io_config);

    init_wifi();
    esp_err_t ret = connect_wifi(WIFI_SSID, WIFI_PASSWORD);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        return;
    }

    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA"), &ip_info);
    ESP_LOGI(TAG, "Server running on address: " IPSTR, IP2STR(&ip_info.ip));

    // Init REST API
    httpd_handle_t server_handle = start_webserver();

    int running = 1;
    while(running) {
        vTaskDelay(pdMS_TO_TICKS(8000));
    }

    stop_webserver(server_handle);
    deinit_wifi();
}

