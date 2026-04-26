#include "timer.h"

void Timer3_PWM_Init(void) {
    /* 1. Set the Timer Clock Frequency */
    TIM3->PSC = 15;   /* Prescaler: 16MHz / (15 + 1) = 1MHz counter clock */
    TIM3->ARR = 100;  /* Auto-Reload: 1MHz / 100 = 10kHz PWM frequency */

    /* 2. Configure Channel 1 for PWM Mode 1 */
    /* Clear Output Compare Mode bits (Bits 6:4) */
    TIM3->CCMR1 &= ~(0x7 << 4);
    
    /* Set Output Compare Mode to PWM Mode 1 (Binary 110 = 0x6) */
    TIM3->CCMR1 |= (0x6 << 4);

    /* Enable Output Compare Preload (Bit 3) */
    SET_BIT(TIM3->CCMR1, 3);

    /* 3. Enable the Capture/Compare output for Channel 1 */
    SET_BIT(TIM3->CCER, 0);

    /* 4. Initialize Duty Cycle to 0% (Fan Off) */
    TIM3->CCR1 = 0;

    /* 5. Enable the Timer Counter (Bit 0 of CR1) */
    SET_BIT(TIM3->CR1, 0);
}

void Timer3_SetPWM_DutyCycle(uint8 duty_percent) {
    /* Safety Cap: Ensure we don't overflow the 100% ARR limit */
    if (duty_percent > 100) {
        duty_percent = 100;
    }
    
    /* Directly write the percentage to the Capture/Compare Register */
    TIM3->CCR1 = duty_percent;
}