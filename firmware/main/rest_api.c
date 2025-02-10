#include "rest_api.h"
#include "sensor_api.h"
#include <driver/gpio.h>
#include "motor_api.h"
#include "io_config.h"
#include <esp_log.h>

// HELPERS
#define TAG "Rest API"



esp_err_t handle_motor_speed(char* data) {
    // 
    uintmax_t duty = strtoumax(data, NULL, 10);
    if (duty > 1023){
       ESP_LOGE(TAG, "Could not convert post request content to motor speed");
       return ESP_FAIL;
    }
    if (test_motor_a(duty) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to spin motor");
        return ESP_FAIL;
    }
    return ESP_OK;
}

static esp_err_t get_handler(httpd_req_t *req) {
    // Indicate transmission time with onboard LED
    gpio_set_level(LED_PIN, 1);
    char response[128] = "ESP32 uri endpoint";
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    gpio_set_level(LED_PIN, 0);
    return ESP_OK;
}

#define MIN(a, b) (a < b ? a: b)
static esp_err_t post_handler(httpd_req_t *req){
    char content[100];
    size_t recv_size = MIN(req->content_len, sizeof(content));
    int ret = httpd_req_recv(req, content, recv_size);
    
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        } 
        return ESP_FAIL; 
    }

    ret = handle_motor_speed(content);
    if (ret != ESP_OK) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    const char resp[] = "URI POST Response";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static int vec3_to_json(char* buffer, size_t buffer_size, const vec3_f* vec) {
    return snprintf(buffer, buffer_size, 
                   "{\"x\":%.3f,\"y\":%.3f,\"z\":%.3f}", 
                   vec->x, vec->y, vec->z);
}

static esp_err_t imu_get_handler(httpd_req_t *req) {
    // Indicate transmission time with onboard LED
    gpio_set_level(LED_PIN, 1);
    char response[128];
    memset(response, 0x00, 128); 

    vec3_f sensor_value = get_gyroscope_data();
    vec3_to_json(response, 128, &sensor_value);
    
    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    gpio_set_level(LED_PIN, 0);
    return ESP_OK;
}

static httpd_uri_t imu_get = {
    .uri = "/imu",
    .method = HTTP_GET,
    .handler = imu_get_handler, 
    .user_ctx = NULL
};

static httpd_uri_t uri_get = {
    .uri = "/uri",
    .method = HTTP_GET,
    .handler = get_handler, 
    .user_ctx = NULL
};

static httpd_uri_t uri_post = {
    .uri = "/uri",
    .method = HTTP_POST,
    .handler = post_handler,
    .user_ctx = NULL
};

// API
httpd_handle_t start_webserver() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
        httpd_register_uri_handler(server, &imu_get);
    }
    return server;
}

void stop_webserver(httpd_handle_t server) {
    if (server != NULL) {
        httpd_stop(server);
    }
}
