#ifndef __ADL_UART_H___
#define __ADL_UART_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define ARDUINO_UART 	UART_NUM_2
#define ARDUINO_BAUD 	9600

void uart_arduino_init(void);

#endif
