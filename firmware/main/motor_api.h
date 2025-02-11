// This file conatins api for controlling both DC motors
//


#pragma once

#include "driver/gpio.h"
#include "io_config.h"


typedef enum {
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_STOP
} motor_direction_t;

// Initialize the motor PWM config;
esp_err_t init_motor_config(void);

// Set motor A speed and direction
esp_err_t set_motor_a(motor_direction_t direction, uint32_t duty);


// Set motor B speed and direction
esp_err_t set_motor_b(motor_direction_t direction, uint32_t duty);
