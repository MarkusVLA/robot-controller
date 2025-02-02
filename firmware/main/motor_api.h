// This file conatins api for controlling both DC motors
//


#pragma once

#include "driver/gpio.h"
#include "io_config.h"

// Initialize the motor PWM config;
esp_err_t init_motor_config(void);
// Set the speed of motor 1
esp_err_t test_motor_a(uint32_t duty);

