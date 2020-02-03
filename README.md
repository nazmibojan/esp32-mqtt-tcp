# ESP32-MQTT publish sample

This sample uses ESP-MQTT library which implements mqtt client to connect to mqtt broker. It will publish current time which get from ntp server.

## How to use example

### Hardware Required

This example can be executed on any ESP32 board, the only required interface is WiFi and connection to internet.

### Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py build
idf.py -p [COM] flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output
```
I (613) wifi: AP's beacon interval = 102400 us, DTIM period = 1
I (1223) event: sta ip: 20.4.110.85, mask: 255.255.255.0, gw: 20.4.110.1
I (1223) system_api: Base MAC address is not set, read default base MAC address from BLK0 of EFUSE
I (1233) ntp.c: Initializing SNTP
I (1233) MQTT_EXAMPLE: Other event id:7
I (1243) ntp.c: Time is not set yet. Getting time over NTP.
I (1243) ntp.c: Waiting for system time to be set... (1/5)
I (2563) MQTT_CLIENT: Sending MQTT CONNECT message, type: 1, id: 0000
I (2873) MQTT_EXAMPLE: MQTT_EVENT_CONNECTED
I (2883) MQTT_EXAMPLE: sent subscribe successful, msg_id=22233
I (3253) MQTT_EXAMPLE: MQTT_EVENT_SUBSCRIBED, msg_id=22233
2020/2/3 17:16:5
I (3603) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=25061
2020/2/3 17:16:15
I (13633) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=42356
2020/2/3 17:16:25
I (23563) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=44928
2020/2/3 17:16:35
I (33593) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=43087
2020/2/3 17:16:45
I (43633) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=36805
2020/2/3 17:16:55
I (53563) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=50469
2020/2/3 17:17:5
I (63593) MQTT_EXAMPLE: MQTT_EVENT_PUBLISHED, msg_id=49868
```
