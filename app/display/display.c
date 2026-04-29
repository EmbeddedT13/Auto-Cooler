#include "display.h"
#include "../../hal/lcd/lcd.h"
#include "../../mcal/gpio/gpio.h"
#include "../../mcal/rcc/rcc.h"

static LCD_Config_t SystemLCD;

void DISPLAY_Init(void) {
    RCC_EnableClock(RCC_AHB1, GPIOB_AHB1_BIT);
    
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

    LCD_SetCursor(&SystemLCD, 0, 0);
    LCD_SendString(&SystemLCD, "System Ready    ");
}

void DISPLAY_Update(uint16 temp_x10, uint8 fan_speed, uint8 is_overheat) {
    uint8 int_part = temp_x10 / 10;
    uint8 frac_part = temp_x10 % 10;

    /* --- Line 1: Temperature --- */
    LCD_SetCursor(&SystemLCD, 0, 0);
    LCD_SendString(&SystemLCD, "Temp: ");
    
    /* 3-Digit Overflow Fix */
    if (int_part >= 100) {
        LCD_SendData(&SystemLCD, (int_part / 100) + '0');       /* Hundreds */
    } else {
        LCD_SendData(&SystemLCD, ' ');                          /* Padding */
    }
    LCD_SendData(&SystemLCD, ((int_part / 10) % 10) + '0');     /* Tens */
    LCD_SendData(&SystemLCD, (int_part % 10) + '0');            /* Ones */
    
    LCD_SendData(&SystemLCD, '.');
    LCD_SendData(&SystemLCD, frac_part + '0');
    LCD_SendString(&SystemLCD, " C  ");

    /* --- Line 2: Fan Speed or Overheat --- */
    LCD_SetCursor(&SystemLCD, 1, 0);
    
    if (is_overheat) {
        LCD_SendString(&SystemLCD, "OVERHEAT!       ");
    } else {
        LCD_SendString(&SystemLCD, "Fan: ");
        
        if (fan_speed >= 100) {
            LCD_SendString(&SystemLCD, "100%       ");
        } else {
            if (fan_speed >= 10) {
                LCD_SendData(&SystemLCD, (fan_speed / 10) + '0');
            } else {
                LCD_SendData(&SystemLCD, ' '); /* Padding */
            }
            LCD_SendData(&SystemLCD, (fan_speed % 10) + '0');
            LCD_SendString(&SystemLCD, "%        ");
        }
    }
}