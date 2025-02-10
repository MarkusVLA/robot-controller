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
    ret = LSM6DS3_init(LSM6DS3_handle);
    

    vec3_16i sensor_data = {0};
    int running = 1;
    while(running) {
        vTaskDelay(pdMS_TO_TICKS(250));
        // test reading the accelerometer
        read_LSM6DS3_gyroscope(LSM6DS3_handle, &sensor_data);
        float x, y, z;
        x = (float) sensor_data.x * 4.0 / 32768.0;
        y = (float) sensor_data.y * 4.0 / 32768.0;
        z = (float) sensor_data.z * 4.0 / 32768.0;
        printf("x: %.2f, y: %.2f, z: %.2f\n", x, y, z);

    }

    stop_webserver(server_handle);
    deinit_wifi();
}

