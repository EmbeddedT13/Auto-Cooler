#include "lcd.h"

/* Basic delay (until we set up a hardware timer) */
static void LCD_Delay(volatile uint32 count) {
    while(count--) {
        __asm("NOP");
    }
}

/* The Heartbeat: Tell the LCD to read the data lines */
static void LCD_PulseEnable(void) {
    GPIO_WritePin(LCD_PORT, LCD_EN_PIN, GPIO_STATE_HIGH);
    LCD_Delay(2000); 
    GPIO_WritePin(LCD_PORT, LCD_EN_PIN, GPIO_STATE_LOW);
    LCD_Delay(2000); 
}

/* Send 4 bits to D4-D7 */
static void LCD_SendNibble(uint8 nibble) {
    GPIO_WritePin(LCD_PORT, LCD_D4_PIN, READ_BIT(nibble, 0));
    GPIO_WritePin(LCD_PORT, LCD_D5_PIN, READ_BIT(nibble, 1));
    GPIO_WritePin(LCD_PORT, LCD_D6_PIN, READ_BIT(nibble, 2));
    GPIO_WritePin(LCD_PORT, LCD_D7_PIN, READ_BIT(nibble, 3));
    LCD_PulseEnable();
}

/* Public API Implementation  */

void LCD_SendCommand(uint8 Command) {
    GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_STATE_LOW); /* RS = 0 for Command */
    
    LCD_SendNibble(Command >> 4);   
    LCD_SendNibble(Command & 0x0F); 
    
    LCD_Delay(5000); 
}

void LCD_SendData(uint8 Data) {
    GPIO_WritePin(LCD_PORT, LCD_RS_PIN, GPIO_STATE_HIGH); /* RS = 1 for Data */
    
    LCD_SendNibble(Data >> 4);   
    LCD_SendNibble(Data & 0x0F); 
    
    LCD_Delay(5000);
}

void LCD_Init(void) {
    /* 1. Initialize all 7 pins as OUTPUTs */
    GPIO_SetPinMode(LCD_PORT, LCD_RS_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LCD_PORT, LCD_RW_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LCD_PORT, LCD_EN_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LCD_PORT, LCD_D4_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LCD_PORT, LCD_D5_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LCD_PORT, LCD_D6_PIN, GPIO_MODE_OUTPUT);
    GPIO_SetPinMode(LCD_PORT, LCD_D7_PIN, GPIO_MODE_OUTPUT);

    /* RW is permanently Write */
    GPIO_WritePin(LCD_PORT, LCD_RW_PIN, GPIO_STATE_LOW);
    GPIO_WritePin(LCD_PORT, LCD_EN_PIN, GPIO_STATE_LOW);

    LCD_Delay(100000); 

    /* 2. The HD44780 Datasheet "Wake Up" Sequence for 4-bit mode */
    LCD_SendCommand(0x33); 
    LCD_SendCommand(0x32); 
    
    /* 3. Configure Display settings */
    LCD_SendCommand(LCD_CMD_4BIT_2LINE);
    LCD_SendCommand(LCD_CMD_DISPLAY_ON);
    LCD_SendCommand(LCD_CMD_ENTRY_MODE);
    LCD_Clear();
}

void LCD_SendString(const char* Str) {
    while(*Str != '\0') {
        LCD_SendData(*Str);
        Str++;
    }
}

void LCD_SetCursor(uint8 Row, uint8 Col) {
    uint8 address;
    if(Row == 0) {
        address = 0x80 + Col; /* Row 0 hardware memory offset */
    } else {
        address = 0xC0 + Col; /* Row 1 hardware memory offset */
    }
    LCD_SendCommand(address);
}

void LCD_Clear(void) {
    LCD_SendCommand(LCD_CMD_CLEAR);
    LCD_Delay(20000); /* Clearing takes physical time to wipe the screen */
}