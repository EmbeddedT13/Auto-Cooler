#include "led.h"

/*to set the designated LED pin as an output.*/
void LED_Init(uint8 LedPin)
{
    if(LedPin <= 15)
    {
        GPIO_SetPinMode(LED_PORT, LedPin, GPIO_MODE_OUTPUT);
    }
}

/*outputs a HIGH signal to turn the LED on.*/
void LED_On(uint8 LedPin)
{
    if(LedPin <= 15)
    {
        GPIO_WritePin(LED_PORT, LedPin, GPIO_STATE_HIGH);
    }
}

/*outputs a LOW signal to turn the LED off.*/
void LED_Off(uint8 LedPin)
{
    if(LedPin <= 15)
    {
        GPIO_WritePin(LED_PORT, LedPin, GPIO_STATE_LOW);
    }
}

/*to read the current state of the LED and flips it.*/
void LED_Toggle(uint8 LedPin)
{
    if(LedPin <= 15)
    {
        
        uint8 currentState = GPIO_ReadPin(LED_PORT, LedPin);
        
        if(currentState == GPIO_STATE_HIGH)
        {
            GPIO_WritePin(LED_PORT, LedPin, GPIO_STATE_LOW);
        }
        else
        {
            GPIO_WritePin(LED_PORT, LedPin, GPIO_STATE_HIGH);
        }
    }
}