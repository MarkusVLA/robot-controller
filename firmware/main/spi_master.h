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
                   
// Initalize the SPI driver with given config
spi_device_handle_t init_spi();

// Test handshake with LSM6DS3
esp_err_t test_LSM6DS3_connection(spi_device_handle_t spi);
