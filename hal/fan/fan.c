#include "fan.h"
#include "../../mcal/timer/timer.h"
#include "../../mcal/rcc/rcc.h"
#include "../../mcal/gpio/gpio.h"

void FAN_Init(void){
    
    RCC_EnableClock(RCC_APB1, RCC_APB1_TIM3_BIT); /*Enable Timer 3 clock on APB1 bus*/

    /* Map PA6 to Timer 3 Alternate Function (AF2) for Proteus simulation */
    RCC_EnableClock(RCC_AHB1, GPIOA_AHB1_BIT);
    GPIO_SetPinMode(GPIOA, 6, GPIO_MODE_ALTERNATE);
    GPIO_SetAlternateFunction(GPIOA, 6, 2);

    Timer3_PWM_Init(); /* Configure the hardware PWM settings */

    FAN_SetSpeed(FAN_SPEED_OFF); /* Ensure the fan is off at startup for safety */
}

void FAN_SetSpeed(uint8 SpeedPercent){

    Timer3_SetPWM_DutyCycle(SpeedPercent); /*Set the speed percent*/
}