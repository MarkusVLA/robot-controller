#include "spi_master.h"
#include "LSM6DS3.h"
#include <string.h>
#include "esp_log.h"

#define TAG "LSM6DS3"

// Sensor read based on sensor inturrupts: INT_1, INT_2

static const gpio_config_t LSM6DS3_intr_config = {
    .pin_bit_mask = ((1ULL << INT_1)  |
                     (1ULL << INT_2)),

    .mode = GPIO_MODE_INPUT,                    
    .pull_up_en = false
};

spi_device_handle_t init_spi(){
    esp_err_t ret;

    ret = gpio_config(&LSM6DS3_intr_config);
    ESP_ERROR_CHECK(ret);

    spi_device_handle_t spi;

    spi_bus_config_t bus_config = {
        .miso_io_num = MISO,
        .mosi_io_num = MOSI,
        .sclk_io_num = SCLK,
        .data_io_default_level = true, // 1
        .max_transfer_sz = 4096
    };

    spi_device_interface_config_t dev_config= {
        .clock_speed_hz = SPI_SCK_SPEED,
        .mode = 3,
        .spics_io_num = CS,
        .queue_size = SPI_QUEUE_SIZE,
        .pre_cb = NULL // Pre transfer callback
    };

    ret = spi_bus_initialize(LSM6DS3_HOST, &bus_config, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(LSM6DS3_HOST, &dev_config, &spi);
    ESP_ERROR_CHECK(ret);
    return spi;
}

static esp_err_t write_LSM6DS3_register(spi_device_handle_t spi, 
                                        uint8_t addr, 
                                        uint8_t val){

    esp_err_t ret;
    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));
    transaction.length = 2 * sizeof(uint8_t) * 8; // = 16: CMD and DATA byte
    

    uint8_t tx_data[2] = {0};
    tx_data[0] = (0x00 | addr); 
    tx_data[1] = (0x00 | val);
                                       // Use byte array if this causes issues
    transaction.tx_buffer = tx_data;
    transaction.rxlength = 0; // No rx data on write reg
    transaction.user = (void*)0;
    // Write to spi driver
    ret = spi_device_polling_transmit(spi, &transaction);
    // Returns ESP_OK when succesful
    return ret;
}

// TODO dynamic read size
static esp_err_t read_LSM6DS3_register(spi_device_handle_t spi, 
                                      uint8_t addr, 
                                      uint8_t *value,
                                      size_t length) {
    esp_err_t ret;
    spi_transaction_t transaction;
    
    memset(&transaction, 0, sizeof(transaction));
    
    uint8_t tx_data[2] = {0};
    tx_data[0] = (0x80 | addr);  // Set read bit 
    
    uint8_t rx_data[2];  // Buffer for command byte + data byte
    
    transaction.length = 16;  // Two bytes (command + data)
    transaction.tx_buffer = tx_data;
    transaction.rx_buffer = rx_data;
    
    ret = spi_device_polling_transmit(spi, &transaction);
    if (ret == ESP_OK) {
        *value = rx_data[1];  // Second byte contains the data
    }

    return ret;
}

static esp_err_t test_LSM6DS3_connection(spi_device_handle_t spi) {
    uint8_t who_am_i;
    esp_err_t ret;

    ret = read_LSM6DS3_register(spi, LSM6DS3_WHO_AM_I, &who_am_i, 1);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read WHO_AM_I register");
        return ret;
    }

    if (who_am_i != LSM6DS3_WHO_AM_I_VALUE) {
        ESP_LOGE(TAG, "WHO_AM_I mismatch. Expected: 0x%x, Got: 0x%x", 
                 LSM6DS3_WHO_AM_I_VALUE, who_am_i);
        return ESP_ERR_INVALID_RESPONSE;
    }

    ESP_LOGI(TAG, "Device ID verified successfully");
    return ESP_OK;
}

esp_err_t LSM6DS3_init(spi_device_handle_t spi){
    esp_err_t ret;
    
    ret = test_LSM6DS3_connection(spi);
    if (ret != ESP_OK){
        return ret; 
    }
    
    ret = write_LSM6DS3_register(spi, LSM6DS3_CTRL2_G,    0x4C);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write register 0x%x!", LSM6DS3_CTRL2_G);
        return ret;
    }

    ret = write_LSM6DS3_register(spi, LSM6DS3_CTRL1_XL,   0x4A);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write register 0x%x!", LSM6DS3_CTRL1_XL);
        return ret;
    }

    ret = write_LSM6DS3_register(spi, LSM6DS3_CTRL7_G,    0x00);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write register 0x%x!", LSM6DS3_CTRL7_G);
        return ret;
    }

    ret = write_LSM6DS3_register(spi, LSM6DS3_CTRL8_XL,   0x09);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write register 0x%x!", LSM6DS3_CTRL8_XL);
        return ret;
    }

    return ESP_OK;
}


// READING DATA FROM THE SENSOR
/*The 1st FIFO data set is reserved for gyroscope data; */
/*The 2nd FIFO data set is reserved for accelerometer data;*/
/*The 3rd FIFO data set is reserved for the external sensor data stored in the registers from*/


/*esp_err_t read_LSM6DS3_accelormeter(spi_device_handle_t spi, vec3_u *data){*/
/*    esp_err_t ret;*/
/*    return ret;*/
/*}*/
/**/
/*esp_err_t read_LSM6DS3_gyroscope(spi_device_handle_t spi, vec3_u *data){*/
/*   esp_err_t ret;*/
/*   return ret;*/
/*}*/










