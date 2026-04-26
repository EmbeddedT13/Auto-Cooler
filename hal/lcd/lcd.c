#include "lcd.h"
#include "../../mcal/timer/timer.h" /* Bring in the new SysTick delays */

/* Private Helpers */

static void LCD_PulseEnable(LCD_Config_t* lcd) {
    GPIO_WritePin(lcd->CtrlPort, lcd->EN_Pin, GPIO_STATE_HIGH);
    Timer_Delay_us(2); /* The HD44780 only needs ~1us to latch data */
    GPIO_WritePin(lcd->CtrlPort, lcd->EN_Pin, GPIO_STATE_LOW);
    Timer_Delay_us(2); 
}

/* Send 4 bits to D4-D7 */
static void LCD_SendNibble(LCD_Config_t* lcd, uint8 nibble) {
    GPIO_WritePin(lcd->DataPort, lcd->D4_Pin, READ_BIT(nibble, 0));
    GPIO_WritePin(lcd->DataPort, lcd->D5_Pin, READ_BIT(nibble, 1));
    GPIO_WritePin(lcd->DataPort, lcd->D6_Pin, READ_BIT(nibble, 2));
    GPIO_WritePin(lcd->DataPort, lcd->D7_Pin, READ_BIT(nibble, 3));
    LCD_PulseEnable(lcd);
}

/* Public API Implementation */

void LCD_SendCommand(LCD_Config_t* lcd, uint8 Command) {
    GPIO_WritePin(lcd->CtrlPort, lcd->RS_Pin, GPIO_STATE_LOW); /* RS = 0 for Command */
    
    LCD_SendNibble(lcd, Command >> 4);   
    LCD_SendNibble(lcd, Command & 0x0F); 
    
    Timer_Delay_ms(2); 
}

void LCD_SendData(LCD_Config_t* lcd, uint8 Data) {
    GPIO_WritePin(lcd->CtrlPort, lcd->RS_Pin, GPIO_STATE_HIGH); /* RS = 1 for Data */
    
    LCD_SendNibble(lcd, Data >> 4);   
    LCD_SendNibble(lcd, Data & 0x0F); 
    
    Timer_Delay_us(50); 
}

void LCD_Init(LCD_Config_t* lcd) {
    /* 1. Initialize all 7 pins as OUTPUTs */
    GPIO_SetPinMode(lcd->CtrlPort, lcd->RS_Pin, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(lcd->CtrlPort, lcd->RW_Pin, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(lcd->CtrlPort, lcd->EN_Pin, GPIO_MODE_OUTPUT);
    
    GPIO_SetPinMode(lcd->DataPort, lcd->D4_Pin, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(lcd->DataPort, lcd->D5_Pin, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(lcd->DataPort, lcd->D6_Pin, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(lcd->DataPort, lcd->D7_Pin, GPIO_MODE_OUTPUT);

    /* RW is permanently Write */
    GPIO_WritePin(lcd->CtrlPort, lcd->RW_Pin, GPIO_STATE_LOW);
    GPIO_WritePin(lcd->CtrlPort, lcd->EN_Pin, GPIO_STATE_LOW);

    /* Give LCD time to stabilize after physical power-on */
    Timer_Delay_ms(50); 

    /* 2. The HD44780 Datasheet "Wake Up" Sequence for 4-bit mode */
    LCD_SendCommand(lcd, 0x33); 
    Timer_Delay_ms(5);
    LCD_SendCommand(lcd, 0x32); 
    Timer_Delay_us(150);
    
    /* 3. Configure Display settings */
    LCD_SendCommand(lcd, LCD_CMD_4BIT_2LINE);
    LCD_SendCommand(lcd, LCD_CMD_DISPLAY_ON);
    LCD_SendCommand(lcd, LCD_CMD_ENTRY_MODE);
    LCD_Clear(lcd);
}

void LCD_SendString(LCD_Config_t* lcd, const char* Str) {
    while(*Str != '\0') {
        LCD_SendData(lcd, *Str);
        Str++;
    }
}

void LCD_SetCursor(LCD_Config_t* lcd, uint8 Row, uint8 Col) {
    uint8 address;
    if(Row == 0) {
        address = 0x80 + Col; /* Row 0 hardware memory offset */
    } else {
        address = 0xC0 + Col; /* Row 1 hardware memory offset */
    }
    LCD_SendCommand(lcd, address);
}

void LCD_Clear(LCD_Config_t* lcd) {
    LCD_SendCommand(lcd, LCD_CMD_CLEAR);
    Timer_Delay_ms(3); 
}