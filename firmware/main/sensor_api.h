// This file containse getters for reading sensor data: API for sensor data
//

#pragma once

#include "spi_master.h"
#include "driver/gpio.h"

typedef struct {
    float x, y, z;
} vec3_f;

esp_err_t init_sensors(void);

vec3_f get_gyroscope_data(void);

