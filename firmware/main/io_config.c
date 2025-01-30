
#include "io_config.h"

gpio_config_t io_config = {
    .pin_bit_mask = (
            1ULL << LED_PIN     |
            1ULL << MOTOR_IN1   |
            1ULL << MOTOR_IN2   |
            1ULL << MOTOR_IN2   |
            1ULL << MOTOR_IN4), // 64 bit mask
                                
    .mode = GPIO_MODE_OUTPUT,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .intr_type = GPIO_INTR_DISABLE
}; 
