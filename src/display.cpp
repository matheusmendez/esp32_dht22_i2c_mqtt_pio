#include "display.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void displayInit(void)
{
    // initialize LCD
    lcd.init();
    // turn on LCD backlight                      
    lcd.backlight();
}
//Exibe mensagem recebida como parâmetro
void showDisplay(int line, String msg, bool clear)
{
    //Limpa o display
    if(clear)
    {
        lcd.clear();
    }
    lcd.setCursor(0, line);
    //Escreve na pos 0,25 a mensagem
    lcd.print(msg); 
}