// https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
// https://github.com/winlinvip/SimpleDHT/archive/refs/heads/master.zip
// https://github.com/knolleary/pubsubclient/archive/refs/heads/master.zip

#include <Arduino.h>
#include <SimpleDHT.h>
#include "MQTT_Client.h"
#include "config.h"

// VARS
float temp, humi, last, turnoff;
float last_mqtt = -TIMEOUT_MQTT;
bool control = false;

SimpleDHT22 dht22(DHT22);

//PROTOTYPE
void setup_lcd(void);
void print_lcd(void);
void read_sensor(void);
void showDisplay(int line, String msg, bool clear);
void showClimate(bool success);
bool getClimate(void);
void controlAlarm(void);

void setup(){
    Serial.begin(115200);
    setupDisplay();
    pinMode(BUZZER,OUTPUT);
    digitalWrite(BUZZER,HIGH);
    //Exibe mensagem no display
    showDisplay(0, "Setting up mqtt...", true);
    Serial.println("Setting up mqtt...");
    
    //Inicializa mqtt (conecta o esp com o wifi, configura e conecta com o servidor mqtt)
    if(!mqttInit())
    {        
        delay(3000);
        showDisplay(0, "Failed!", false);
        Serial.println("Failed!");
        ESP.restart();
    }
    
    showDisplay(0, "OK", false);
    Serial.println("OK");
}

void loop(){
    //Se o esp foi desconectado do ubidots, tentamos reconectar
    if(!client.connected())
        reconnect();

    //Lê a temperatura e umidade e exibimos no display passando uma flag (que sinaliza sucesso na leitura)
    if(getClimate())
        showClimate(true);
    else
        showClimate(false);

    //Esperamos 2.5s antes de exibir o status do envio para dar efeito de pisca no display
    delay(2500);
    if(millis()-last_mqtt >= TIMEOUT_MQTT)
    {
        if(sendValues(temp, humi))
        {      
            Serial.println("Successfully sent data");
            //showDisplay(4,"Successfully sent data", false);
        }
        else
        {      
            Serial.println("Failed to send sensor data");
            //showDisplay(4,"Failed to send sensor data", false);
        }
        last_mqtt = millis();
    }
    //Esperamos 2.5s para dar tempo de ler as mensagens acima
    controlAlarm();
    delay(2500); 
}

//Obtém temperatura e umidade do sensor
bool getClimate(void)
{  
    int err = SimpleDHTErrSuccess;

    //Passamos as variáveis 'temperature' e 'humidity' por parâmetro na função chamada 'read2', elas serão retornadas por referência
    //Se não for retornado o código de SimpleDHTErrSuccess (sucesso), exibimos o valor de erro obtido
    if ((err = dht22.read2(&temp, &humi, NULL)) != SimpleDHTErrSuccess) 
    {
        Serial.print("Read DHT22 failed, err=");
        Serial.println(err);

        //Zera valores
        temp = humi = 0;
        return false;
    }

    return true;
}

//Essa função exibe os valores com suas respectivas unidades de medida para caso a flag 'success' seja true
//Caso a flag 'success' seja false, exibimos apenas um traço '-'
void showClimate(bool success)
{
    if(success)
    {
        showDisplay(0, "Temp: "+String(temp)+ "\xDF" +"C", true);
        showDisplay(1, "Humi: "+String(humi)+" %", false);
        Serial.println("Temp: "+String(temp)+"°C");
        Serial.println("Humi: "+String(humi)+" %");
    }
    else
    {
        showDisplay(0, "Temp: -", true);
        showDisplay(1, "Humi: -", false);
        Serial.println("Temp: -");
        Serial.println("Humi: -");
    }
}

void controlAlarm(void)
{
    if (temp > TEMP_MAX|| temp < TEMP_MIN|| humi > HUMI_MAX || humi < HUMI_MIN )
    {
        if(!control)
        {
            digitalWrite(BUZZER,LOW);
            last = millis();
            control = true;
        }
        else
        {
            if(millis()-last >= TIMEOUT)
            {
                control = false;
            }
            if(millis()-turnoff >= 5000)
            {
                digitalWrite(BUZZER,HIGH);
                turnoff = millis() + TIMEOUT;
            }
        }
    }
}