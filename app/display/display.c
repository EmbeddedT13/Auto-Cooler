#include "display.h"
#include "../../hal/lcd/lcd.h"
#include "../../mcal/gpio/gpio.h"

/* Private instance of the LCD configuration */
static LCD_Config_t SystemLCD;

void DISPLAY_Init(void) {
    /* Initialize LCD based on Proteus Wiring */
    SystemLCD.CtrlPort = GPIOB;
    SystemLCD.DataPort = GPIOB;
    SystemLCD.RS_Pin   = GPIO_PIN00;
    SystemLCD.RW_Pin   = GPIO_PIN01;
    SystemLCD.EN_Pin   = GPIO_PIN02;
    SystemLCD.D4_Pin   = GPIO_PIN04;
    SystemLCD.D5_Pin   = GPIO_PIN05;
    SystemLCD.D6_Pin   = GPIO_PIN06;
    SystemLCD.D7_Pin   = GPIO_PIN07;
    
    LCD_Init(&SystemLCD);

    /* Setup initial display */
    LCD_SetCursor(&SystemLCD, 0, 0);
    LCD_SendString(&SystemLCD, (uint8*)"System Ready");
}

void DISPLAY_Update(uint16 temp_x10, uint8 fan_speed, uint8 is_overheat) {
    /* Clear the screen of any old data */
    LCD_Clear(&SystemLCD);

    /* Calculate integer and fractional parts of the temperature */
    uint8 int_part = temp_x10 / 10;
    uint8 frac_part = temp_x10 % 10;

    /* --- Line 1: Temperature --- */
    LCD_SetCursor(&SystemLCD, 0, 0);
    LCD_SendString(&SystemLCD, (uint8*)"Temp: ");
    
    /* Send the tens digit, ones digit, decimal point, and fraction digit */
    LCD_SendData(&SystemLCD, (int_part / 10) + '0');
    LCD_SendData(&SystemLCD, (int_part % 10) + '0');
    LCD_SendData(&SystemLCD, '.');
    LCD_SendData(&SystemLCD, frac_part + '0');
    LCD_SendString(&SystemLCD, (uint8*)" C");

    /* --- Line 2: Fan Speed or Overheat --- */
    LCD_SetCursor(&SystemLCD, 1, 0);
    
    if (is_overheat) {
        LCD_SendString(&SystemLCD, (uint8*)"OVERHEAT!");
    } else {
        LCD_SendString(&SystemLCD, (uint8*)"Fan: ");
        
        /* Print the fan speed */
        if (fan_speed == 100) {
            LCD_SendString(&SystemLCD, (uint8*)"100");
        } else {
            /* If the speed is two digits (like 33 or 66), print the tens digit */
            if (fan_speed >= 10) {
                LCD_SendData(&SystemLCD, (fan_speed / 10) + '0');
            }
            /* Print the ones digit */
            LCD_SendData(&SystemLCD, (fan_speed % 10) + '0');
        }
        LCD_SendString(&SystemLCD, (uint8*)"%");
    }
}