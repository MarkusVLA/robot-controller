// GPIO pin definitions 

#pragma once

#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_39

#define MOTOR_A_IN1     GPIO_NUM_34
#define MOTOR_A_IN2     GPIO_NUM_33
#define MOTOR_B_IN1     GPIO_NUM_37
#define MOTOR_B_IN2     GPIO_NUM_36

#define MOTOR_SLEEP     GPIO_NUM_35 

// Io configuration structure. 
extern gpio_config_t io_config;

