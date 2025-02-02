#include "motor_api.h"
#include "driver/ledc.h" // PWM for motor speed
#include "io_config.h"

#define TAG "Motor Driver"

static const ledc_timer_config_t motor_timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_10_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 5000,
    .clk_cfg = LEDC_AUTO_CLK,
};

static const ledc_channel_config_t motor_channel_config = {
    .gpio_num = MOTOR_IN1,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .intr_type = LEDC_INTR_DISABLE,
    .duty = 0,
    .hpoint = 0 
};

esp_err_t init_motor_config(void) {
    esp_err_t ret;
    ret = ledc_timer_config(&motor_timer_config);
    
    if (ret != ESP_OK){
        return ret;
    } 
    ret = ledc_channel_config(&motor_channel_config);
    return ret;
}

esp_err_t test_motor_a(uint32_t duty) { 
    esp_err_t ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);

    if (ret != ESP_OK){
        return ret;
    }
    ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    return ret;
}
