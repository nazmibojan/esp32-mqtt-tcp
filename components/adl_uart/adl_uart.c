#include "adl_uart.h"
#include "adl_mqtt.h"

#define BUF_SIZE (1024)
#define RD_BUF_SIZE (BUF_SIZE)

static const char *TAG = "adl_uart.c";
QueueHandle_t uart_arduino_queue = NULL;

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(RD_BUF_SIZE);
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart_arduino_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            bzero(dtmp, RD_BUF_SIZE);
            ESP_LOGI(TAG, "uart[%d] event:", ARDUINO_UART);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:
                    ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                    uart_read_bytes(ARDUINO_UART, dtmp, event.size, portMAX_DELAY);
                    ESP_LOGI(TAG, "[DATA EVT]: %s", dtmp);
                    mqtt_pub_relay_status(dtmp);
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(ARDUINO_UART);
                    xQueueReset(uart_arduino_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(ARDUINO_UART);
                    xQueueReset(uart_arduino_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    // uart_get_buffered_data_len(ARDUINO_UART, &buffered_size);
                    // int pos = uart_pattern_pop_pos(ARDUINO_UART);
                    // ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d, buffered size: %d", pos, buffered_size);
                    // if (pos == -1) {
                    //     // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                    //     // record the position. We should set a larger queue size.
                    //     // As an example, we directly flush the rx buffer here.
                    //     uart_flush_input(ARDUINO_UART);
                    // } else {
                    //     uart_read_bytes(ARDUINO_UART, dtmp, pos, 100 / portTICK_PERIOD_MS);
                    //     uint8_t pat[PATTERN_CHR_NUM + 1];
                    //     memset(pat, 0, sizeof(pat));
                    //     uart_read_bytes(ARDUINO_UART, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                    //     ESP_LOGI(TAG, "read data: %s", dtmp);
                    //     ESP_LOGI(TAG, "read pat : %s", pat);
                    // }
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

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

    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 12, NULL);
}


