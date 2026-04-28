#include "fan.h"
#include "../../mcal/timer/timer.h"
#include "../../mcal/rcc/rcc.h"
#include "../../mcal/gpio/gpio.h"

void FAN_Init(void){
    
    /* Enable Timer 3 clock on APB1 bus */
    RCC_EnableClock(RCC_APB1, RCC_APB1_TIM3_BIT); 

    /* Map PA6 to Timer 3 Alternate Function (AF2) for Proteus simulation */
    RCC_EnableClock(RCC_AHB1, GPIOA_AHB1_BIT);
    GPIO_SetPinMode(GPIOA, 6, GPIO_MODE_ALTERNATE);
    GPIO_SetAlternateFunction(GPIOA, 6, 2);

    /* * Configure TIM3, Channel 1 for PWM.
     * Assuming 16MHz System Clock:
     * PSC = 160 - 1  (Timer clock becomes 100 kHz)
     * ARR = 100 - 1  (PWM frequency = 1 kHz, perfect for L293D motor control)
     * ARR of 100 means a duty cycle value of 0-100 directly matches a percentage!
     */
    Timer_PWM_Init(TIM3, TIMER_CH1, 159, 99); 

    /* Ensure the fan is off at startup for safety */
    FAN_SetSpeed(FAN_SPEED_OFF); 
}

void FAN_SetSpeed(uint8 SpeedPercent){
    
    /* Using the new modular driver, capping at 100% for safety */
    if(SpeedPercent > 100) SpeedPercent = 100;
    
    Timer_SetPWM_Duty(TIM3, TIMER_CH1, SpeedPercent);
}