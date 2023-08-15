// https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
// https://github.com/winlinvip/SimpleDHT/archive/refs/heads/master.zip
// https://github.com/knolleary/pubsubclient/archive/refs/heads/master.zip

#include "main.h"

// VARS
float temp, humi, last, turnoff;
// float last_mqtt = -TIMEOUT_MQTT;
// float last_alarm = -TIMEOUT_ALARM;
float last_mqtt = 0.0F;
float last_alarm = 0.0F;
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
        if(millis()-last_mqtt >= TIMEOUT_MQTT || last_mqtt == 0.0F)
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

    controlAlarm();
    delay(DELAY_LOOP); 
}

void controlAlarm(void)
{
    if(!control && (millis() - last_alarm < TIMEOUT_ALARM || last_mqtt == 0.0F))
    {
        if ((temp > TEMP_MAX || temp < TEMP_MIN|| humi > HUMI_MAX || humi < HUMI_MIN) && (temp != 0.0F && humi != 0.0F))
        {
            //digitalWrite(BUZZER,LOW);
            digitalWrite(BUZZER,HIGH);
            last_alarm = millis();
            control = true;
        }
    }
    else if (millis() - last_alarm >= 5000)
    {
        digitalWrite(BUZZER,LOW);
        control = false;
    }
    
}
// void controlAlarm(void)
// {
//     if ((temp > TEMP_MAX || temp < TEMP_MIN|| humi > HUMI_MAX || humi < HUMI_MIN) && (temp != 0.0F && humi != 0.0F))
//     {
//         if(!control)
//         {
//             //digitalWrite(BUZZER,LOW);
//             digitalWrite(BUZZER,HIGH);
//             last = millis();
//             control = true;
//         }
//         else
//         {
//             if(millis()-last >= TIMEOUT)
//             {
//                 control = false;
//             }
//             if(millis()-turnoff >= 5000)
//             {
//                 //digitalWrite(BUZZER,HIGH);
//                 digitalWrite(BUZZER,LOW);
//                 turnoff = millis() + TIMEOUT;
//             }
//         }
//     }
// }