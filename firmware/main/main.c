#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <math.h>

#include "spi_master.h"

#define LED_PIN GPIO_NUM_14
#define TAG "main"

void app_main(void) {
    // IO configuration setup
    gpio_config_t io_config = {
        .pin_bit_mask = (1ULL << LED_PIN), // 64 bit mask
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    }; 

    gpio_config(&io_config); // Pass gpio_config_t pointer to load the config
    
    int t = 0;
    int running = 1;

    while(running) {
        gpio_set_level(LED_PIN , 1);
        printf("LED on\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(LED_PIN, 0);
        printf("LED off\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        t++;
    }
}

