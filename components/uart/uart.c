#include "uart.h"

QueueHandle_t uart_arduino_queue = NULL;

void uart_arduino_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = ARDUINO_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    //Set UART parameters
    uart_param_config(ARDUINO_UART, &uart_config);
    //Set UART pins
    uart_set_pin(ARDUINO_UART, GPIO_NUM_17, GPIO_NUM_16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    //Install UART driver, and get the queue.
    uart_driver_install(ARDUINO_UART, 1024, 512, 10, &uart_arduino_queue, 0);
}


