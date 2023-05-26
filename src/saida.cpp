#include "saida.h"

void saidaInit(void)
{
    Serial.begin(115200);
    pinMode(BUZZER,OUTPUT);
    digitalWrite(BUZZER,LOW);
}