// This file contains SPI master driver configuration and API for communicating with LSM6DS3TR accelerometer and gyroscope SoC. 

#pragma once 

#include <esp_err.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "driver/spi_master.h"

#define INT_1   GPIO_NUM_9
#define INT_2   GPIO_NUM_14

#define MISO    GPIO_NUM_10
#define MOSI    GPIO_NUM_11
#define SCLK    GPIO_NUM_12
#define CS      GPIO_NUM_13

#define SPI_SCK_SPEED 10e6 // 10 MHz clock out
#define SPI_QUEUE_SIZE 8

#define LSM6DS3_RX_BUF_SIZE (8 * sizeof(uint8_t))
#define LSM6DS3_HOST SPI2_HOST

// Vector3 struct for accelerometer register values.
typedef struct {
    // Has to be converted to degrees.
    uint16_t x, y, z;
} vec3_u;
                   
// Initalize the SPI driver with given config
spi_device_handle_t init_spi();

// Initalize the IMU sensor.
esp_err_t LSM6DS3_init(spi_device_handle_t spi);

// Read the accelerometer in to a vec3_u struct:
esp_err_t read_LSM6DS3_accelormeter(spi_device_handle_t spi, vec3_u *data);
