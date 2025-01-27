idf_component_register(SRCS "main.c" "spi_master.c"
                      INCLUDE_DIRS ""
                      REQUIRES driver esp_wifi esp_event
                      PRIV_REQUIRES nvs_flash esp_netif esp_timer esp_system esp_hw_support)
