// https://www.fernandok.com/2018/11/introducao-ao-protocolo-mqtt-com.html
// https://github.com/knolleary/pubsubclient/archive/refs/heads/master.zip
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "lcdI2C.h"
#include "config.h"

//Objeto WiFiClient usado para a conexão wifi
WiFiClient broker;
PubSubClient client(broker);

bool mqttInit()
{
    //Inicia WiFi com o SSID e a senha
    WiFi.begin(WIFISSID, WIFIPASSWORD);

    //Loop até que o WiFi esteja conectado
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Establishing connection to WiFi..");
        showDisplay(0,"Connected to network", true);
    }

    //Exibe no monitor serial
    Serial.println("Connected to network");
    showDisplay(0,"Connected to network", true);

    //Exibe no monitor serial
    Serial.println("IP:"+String(WiFi.localIP()));
    showDisplay(1,"IP:"+String(WiFi.localIP()), false);
    delay(3000);
    //Seta servidor com o broker e a porta
    client.setServer(SERVER, PORT);

    while(!client.connect(DEVICE_ID, TOKEN, PASSWORD))
    {
        Serial.println("MQTT - Connect error");
        return false;
    }

    Serial.println("MQTT - Connect ok");
    return true;
}

void reconnect() 
{  
    //Loop até que o MQTT esteja conectado
    while (!client.connected()) 
    {
        //sinaliza desconexão do mqtt no display
        showDisplay(0,"Disconnected", true); 
        showDisplay(1, "Trying to reconnect mqtt...", false);
        Serial.println("Attempting MQTT connection...");

        //Tenta conectar
        if (client.connect(DEVICE_ID, TOKEN,PASSWORD))
        {
            Serial.println("connected");
        }
        else 
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 2 seconds");
            //Aguarda 2 segundos antes de retomar
            delay(2000);
        }
    }
    //Sinaliza reconexão do mqtt no display
    showDisplay(0,"Reconnected", true); 
}

//Envia valores por mqtt
//Exemplo: {"id_sensor":"sensor_ambiente_1", "temp_value":24.50, "humi_value":57.20}
bool sendValues(float temperature, float humidity)
{
    char json[250];

    //Atribui para a cadeia de caracteres "json" os valores referentes a temperatura e os envia para a variável do ubidots correspondente
    sprintf(json,  "{\"id_sensor\":\"%s\",\"temp_value\":%02.02f, \"humi_value\":%02.02f}", DEVICE_ID, temperature, temperature, humidity);  

    if(!client.publish(TOPIC, json))
    {
        return false;
    }
    return true;
}