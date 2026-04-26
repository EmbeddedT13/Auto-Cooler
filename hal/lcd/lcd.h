#ifndef LCD_H
#define LCD_H

#include "../../core/STD_TYPES.h"
#include "../../core/BIT_MATH.h"
#include "../../mcal/gpio/gpio.h"

/* 1. The Configuration Blueprint */
typedef struct {
    gpio* DataPort;
    gpio* CtrlPort;      
    uint8 RS_Pin;
    uint8 RW_Pin;
    uint8 EN_Pin;
    uint8 D4_Pin;
    uint8 D5_Pin;
    uint8 D6_Pin;
    uint8 D7_Pin;
} LCD_Config_t;

/* Standard Commands */
#define LCD_CMD_CLEAR           0x01
#define LCD_CMD_RETURN_HOME     0x02
#define LCD_CMD_ENTRY_MODE      0x06 
#define LCD_CMD_DISPLAY_ON      0x0C
#define LCD_CMD_4BIT_2LINE      0x28 

/* 2. Update APIs to require the struct pointer */
void LCD_Init(LCD_Config_t* lcd);
void LCD_SendCommand(LCD_Config_t* lcd, uint8 Command);
void LCD_SendData(LCD_Config_t* lcd, uint8 Data);
void LCD_SendString(LCD_Config_t* lcd, const char* Str);
void LCD_SetCursor(LCD_Config_t* lcd, uint8 Row, uint8 Col);
void LCD_Clear(LCD_Config_t* lcd);

#endif