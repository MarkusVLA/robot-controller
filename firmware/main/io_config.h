// GPIO pin definitions and modes stored here

#pragma once

#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_38

#define MOTOR_IN1 GPIO_NUM_14 
/*#define MOTOR_IN2 GPIO_NUM_18*/
/*#define MOTOR_IN3 GPIO_NUM_17*/
/*#define MOTOR_IN4 GPIO_NUM_14*/

// Io configuration structure. Create more for other configs
extern gpio_config_t io_config;

