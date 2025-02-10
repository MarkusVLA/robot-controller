// This file containse getters for reading sensor data: API for sensor data
//

#pragma once

#include "spi_master.h"
#include "driver/gpio.h"

esp_err_t init_sensors(void);

vec3_16i get_test_data();

