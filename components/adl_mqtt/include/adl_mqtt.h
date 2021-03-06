#ifndef __ADL_MQTT_H___
#define __ADL_MQTT_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "mqtt_client.h"

void mqtt_app_start(void);
void mqtt_pub_relay_status(uint8_t * relay_state);

#endif
