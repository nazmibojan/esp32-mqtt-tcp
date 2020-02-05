#include "adl_mqtt.h"
#include "adl_ntp.h"

#define BROKER_URL "mqtt://nglettrq:RVPcR2AQJEV1@hairdresser.cloudmqtt.com:18848"

esp_mqtt_client_handle_t client = NULL;
char time_date[50];
static const char *TAG = "adl_mqtt.c";
static const char *device_serial = "2286179853734245";

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/ADLDev-1/relay_control", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;

        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = BROKER_URL,
        .event_handle = mqtt_event_handler,
        // .user_context = (void *)your_context
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(client);
}

void mqtt_pub_relay_status(uint8_t * relay_state)
{
    struct tm timenow;
    char * relay = "0";
    char publish_message[1024] = {0};

    if (strcmp((char *)relay_state, "relay_off") == 0) {
        strcpy(relay, "0");
    } else if (strcmp((char *)relay_state, "relay_on") == 0) {
        strcpy(relay, "1");
    } else {
        ESP_LOGW(TAG, "Relay state did not recognized");
        return;
    }

    timenow = ntp_obtain_datetime();
    sprintf(time_date, "%d/%d/%d %d:%d:%d", timenow.tm_year + 1900, timenow.tm_mon + 1, timenow.tm_mday, timenow.tm_hour, timenow.tm_min, timenow.tm_sec);
    ESP_LOGI(TAG, "Timenow: %s", time_date);
    strcat(publish_message, "{\"eventName\":\"relayStatus\",\"status\":\"none\"");
    strcat(publish_message, ",\"relay\":");
    strcat(publish_message, relay);
    strcat(publish_message, ",\"time\":");
    strcat(publish_message, time_date);
    strcat(publish_message, ",\"mac\":\"");
    strcat(publish_message, device_serial);
    strcat(publish_message, "\"}");
    esp_mqtt_client_publish(client, "ADLDev-1/relay_status", publish_message, 0, 1, 0);
}