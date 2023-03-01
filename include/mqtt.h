// https://www.fernandok.com/2018/11/introducao-ao-protocolo-mqtt-com.html
// https://github.com/knolleary/pubsubclient/archive/refs/heads/master.zip

#ifndef _MQTT_H_
#define _MQTT_H_

/*Inclusaão das bibliotecas necessárias*/
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "display.h"
#include "config.h"

/*Protótipos das funções*/
bool mqttInit(void);
void mqttSetup(void);
void reconnect(void);
bool sendValues(float temperature, float humidity);
bool isConnected(void);

#endif