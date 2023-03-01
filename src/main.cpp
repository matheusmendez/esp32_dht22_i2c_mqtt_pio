// https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
// https://github.com/winlinvip/SimpleDHT/archive/refs/heads/master.zip
// https://github.com/knolleary/pubsubclient/archive/refs/heads/master.zip

#include "main.h"

// VARS
float temp, humi, last, turnoff;
float last_mqtt = -TIMEOUT_MQTT;
bool control = false;

void setup(){
    saidaInit();
    displayInit();
    mqttSetup();
}

void loop(){
    //Se o esp foi desconectado do ubidots, tentamos reconectar
    if(!isConnected())
        reconnect();

    //Lê a temperatura e umidade e exibimos no display passando uma flag (que sinaliza sucesso na leitura)
    if(getClimate(&temp,&humi))
    {
        showClimate(true,temp, humi);
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
    }
        
    else
        showClimate(false,temp, humi);

    //Esperamos 2.5s antes de exibir o status do envio para dar efeito de pisca no display
    delay(2500);
    
    //Esperamos 2.5s para dar tempo de ler as mensagens acima
    controlAlarm();
    delay(2500); 
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