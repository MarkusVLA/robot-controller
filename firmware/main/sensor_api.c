// Sensor data abstraction layer for REST. Mainly to simplify the REST API code.

#include "sensor_api.h"
#include "spi_master.h"
#include "esp_log.h"

#define TAG "Sensor API"

static spi_device_handle_t LSM6DS3_handle;

static vec3_f gyro_data_to_unit_vector(vec3_16i sensor_data){
    vec3_f ret = {
        .x = (float) sensor_data.x * 4.0 / 32768,
        .y = (float) sensor_data.y * 4.0 / 32768,
        .z = (float) sensor_data.z * 4.0 / 32768,
    }; return ret;
}

esp_err_t init_sensors(){
    esp_err_t ret;
    LSM6DS3_handle = init_spi(); // Get handle from init method
    ret = LSM6DS3_init(LSM6DS3_handle);

    if (ret != ESP_OK){
        ESP_LOGE(TAG, "Failed to init IMU sensor");
        return ret;
    } 
    return ret;
}

vec3_f get_gyroscope_data(void){
    vec3_16i val = {0};
    read_LSM6DS3_accelormeter(LSM6DS3_handle, &val);
    return gyro_data_to_unit_vector(val);
}



