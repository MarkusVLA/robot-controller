#include "motor_api.h"
#include "driver/ledc.h" // PWM for motor speed
#include "io_config.h"
#include "esp_log.h"

#define TAG "Motor Driver"

static const ledc_timer_config_t motor_timer_config = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_10_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 1000,                // 1kHz Ctrl signal for motors
    .clk_cfg = LEDC_AUTO_CLK,
};

static const ledc_channel_config_t motor_channels[4] = {

    {
        .gpio_num = MOTOR_A_IN1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,
        .hpoint = 0 
    },

    {
        .gpio_num = MOTOR_A_IN2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_1,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,
        .hpoint = 0 
    },

    {
        .gpio_num = MOTOR_B_IN1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_2,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,
        .hpoint = 0 
    },

    {
        .gpio_num = MOTOR_B_IN2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_3,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .duty = 0,
        .hpoint = 0 
    },
};

esp_err_t init_motor_config(void) {
    esp_err_t ret;
    ret = ledc_timer_config(&motor_timer_config);
    
    if (ret != ESP_OK){
        return ret;
    } 

    for (int i = 0; i < 4; i++){
        ret = ledc_channel_config(&motor_channels[i]);
        if (ret != ESP_OK){
            ESP_LOGE(TAG, "Failed to init motor input %d", i);
            return ret;
        }
    }

    return ESP_OK;
}

/*esp_err_t test_motor_a(uint32_t duty) { */
/*    esp_err_t ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty);*/
/**/
/*    if (ret != ESP_OK){*/
/*        return ret;*/
/*    }*/
/**/
/*    ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);*/
/*    return ret;*/
/*}*/


esp_err_t set_motor_speed(int motor_num, motor_direction_t direction, uint32_t duty) {
    esp_err_t ret = ESP_OK;
    int channel_offset = motor_num * 2;  // 0 for motor A, 2 for motor B
    
    switch (direction) {
        case MOTOR_FORWARD:
            // Set IN1 to duty and IN2 to 0
            ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset].channel, duty);
            if (ret != ESP_OK) return ret;
            ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset + 1].channel, 0);
            if (ret != ESP_OK) return ret;
            break;
            
        case MOTOR_BACKWARD:
            // Set IN1 to 0 and IN2 to duty
            ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset].channel, 0);
            if (ret != ESP_OK) return ret;
            ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset + 1].channel, duty);
            if (ret != ESP_OK) return ret;
            break;
            
        case MOTOR_STOP:
            // Set both pins to 0
            ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset].channel, 0);
            if (ret != ESP_OK) return ret;
            ret = ledc_set_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset + 1].channel, 0);
            if (ret != ESP_OK) return ret;
            break;
    }
    
    ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset].channel);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set motor speed");
        return ret;
    }
    
    ret = ledc_update_duty(LEDC_LOW_SPEED_MODE, motor_channels[channel_offset + 1].channel);
    
    return ret;
}

esp_err_t set_motor_a(motor_direction_t direction, uint32_t duty) {
    return set_motor_speed(0, direction, duty);
}

esp_err_t set_motor_b(motor_direction_t direction, uint32_t duty) {
    return set_motor_speed(1, direction, duty);
}

