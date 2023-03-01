#include "sensor.h"

SimpleDHT22 dht22(DHT22);

//Obtém temperatura e umidade do sensor
bool getClimate(float* temp,float* humi)
{  
    int err = SimpleDHTErrSuccess;

    //Passamos as variáveis 'temperature' e 'humidity' por parâmetro na função chamada 'read2', elas serão retornadas por referência
    //Se não for retornado o código de SimpleDHTErrSuccess (sucesso), exibimos o valor de erro obtido
    if ((err = dht22.read2(temp, humi, NULL)) != SimpleDHTErrSuccess) 
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
void showClimate(bool success,float temp, float humi)
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