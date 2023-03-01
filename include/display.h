// https://github.com/marcoschwartz/LiquidCrystal_I2C/archive/master.zip
#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <Arduino.h>

/*Biblioteca do display I2C*/
#include <LiquidCrystal_I2C.h>

/*Definição das constantes*/
#define LCD_COLUMNS 16
#define LCD_ROWS  2
#define LCD_ADDRESS 0x27

/*Protótipos das funções*/
void displayInit(void);
void showDisplay(int line, String msg, bool clear);
#endif