#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_
#include "user_interface.h"

#define CFG_HOLDER	0x00FF55A1
#define CFG_LOCATION	0x3C

#define MQTT_HOST	"m11.cloudmqtt.com" //or "mqtt.domain.com
#define MQTT_PORT	10596
#define MQTT_BUF_SIZE	1024

#define MQTT_CLIENT_ID		"ESP8266_%8X"
#define MQTT_USER		"MQTTUSER"
#define MQTT_PASS		"MQTTPASS"
#define MQTT_SUB_TOPIC_NUM	1


#define OTA_HOST	MQTT_HOST
#define OTA_PORT	80

#define KEY "39cdfe29a1863489e788"

#define AP_SSID "DVES_%08X"
#define AP_PASS "dves"
#define AP_TYPE AUTH_OPEN

#define STA_SSID "ssid"  // WIFI SSID
#define STA_PASS "pwd" // WIFI PASSWORD
#define STA_TYPE AUTH_WPA2_PSK

#define MQTT_RECONNECT_TIMEOUT 5
#define MQTT_CONNTECT_TIMER 5
#endif
