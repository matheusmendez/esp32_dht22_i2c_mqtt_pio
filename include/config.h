#ifndef _CONFIG_H_
#define _CONFIG_H_

//CONSTANTS
#define DHT22 4
#define BUZZER 2
#define TEMP_MAX 30
#define TEMP_MIN 18
#define HUMI_MAX 60
#define HUMI_MIN 30
#define TIMEOUT 900000
#define TIMEOUT_MQTT 60000
#define DELAY_LOOP 20000
#define TEMP_OFFSET 0.5
#define HUMI_OFFSET 16.5


//WIFI
#define WIFISSID "MULTIWIFI"//Coloque seu SSID de WiFi aqui
#define WIFIPASSWORD "multilaser"//Coloque seu password de WiFi aqui
//MQTT
#define SERVER "10.70.40.247"//IP do Servidor MQTT (broker)
#define DEVICE_ID "sensor_ambiente_1" //ID do dispositivo (Device id, também chamado de client name)
#define TOKEN ""//"xxxxxx" //Coloque seu TOKEN do Ubidots aqui
#define PASSWORD ""
//Porta padrão
#define PORT 1883
//Tópico aonde serão feitos os publish
#define TOPIC "smt"

#endif