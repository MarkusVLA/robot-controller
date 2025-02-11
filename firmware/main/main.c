#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_log.h>

#include "wifi_api.h"
#include "rest_api.h"
#include "motor_api.h"
#include "sensor_api.h"
#include "io_config.h"

// Private config should include the defenitions:
// WIFI_SSID and WIFI_PASSWORD
#include "private_config.h" 

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

    ret = init_sensors();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initalize sensors");
    }

    int running = 1;
    while(running) {
        vTaskDelay(pdMS_TO_TICKS(250));
        set_motor_a(MOTOR_FORWARD, 512); // Motor A forwards at 50% speed;
    }

    stop_webserver(server_handle);
    deinit_wifi();

}
