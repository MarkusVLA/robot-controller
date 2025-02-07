// This file contains SPI master driver configuration and API for communicating with LSM6DS3TR accelerometer and gyroscope SoC. 

#pragma once 

#include <esp_err.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/spi_common.h"
#include "hal/spi_types.h"

#define INT_1   GPIO_NUM_9
#define INT_2   GPIO_NUM_14
#define SDO     GPIO_NUM_10
#define SDA     GPIO_NUM_11
#define SCL     GPIO_NUM_12
#define CS      GPIO_NUM_13

#define SPI_SCL_SPEED 1e6 // 1MHz clock out
                         
                          
// Command for accelerometer           
typedef struct {
    uint8_t cmd;
}LSM6DS3_cmd;

// IO configuration for SPI bus
extern gpio_config_t spi_io_config;

// Initalize the SPI driver with given config
esp_err_t init_spi(spi_bus_config_t config);

// Transmit byte array through spi and save response in rec
esp_err_t spi_transmit_bytes(uint8_t *bytes, uint8_t *rec);

// Deinitialize the SPI driver.
esp_err_t deinit_spi(void);




