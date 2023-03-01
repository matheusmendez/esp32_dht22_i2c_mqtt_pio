#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>
#include <SimpleDHT.h>
#include "mqtt.h"
#include "config.h"


void showClimate(bool success,float temp,float humi);
bool getClimate(float* temp,float* humi);

#endif