idf_component_register(SRCS "main.c" "spi_master.c" "wifi_api.c" "rest_api.c" "sensor_api.c" "io_config.c" "motor_api.c"
                      INCLUDE_DIRS ""
                      REQUIRES driver esp_wifi esp_event esp_http_server esp_driver_ledc
                      PRIV_REQUIRES nvs_flash esp_netif esp_timer esp_system esp_hw_support)

