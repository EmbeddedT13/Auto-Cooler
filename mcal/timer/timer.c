#include "timer.h"
#include "../rcc/rcc.h"

void Timer_PWM_Init(timer* TIMx, uint8 Channel, uint16 PSC_Value, uint16 ARR_Value) {
    /* 1. Power on the specific timer */
    if(TIMx == TIM2)      { RCC_EnableTIM2(); }
    else if(TIMx == TIM3) { RCC_EnableTIM3(); }
    else if(TIMx == TIM4) { RCC_EnableTIM4(); }
    /* Note: Add RCC_Enable macros to your rcc.h as needed! */

    /* 2. Set Frequency */
    TIMx->PSC = PSC_Value;
    TIMx->ARR = ARR_Value;

    /* 3. Route the specific Channel */
    switch(Channel) {
        case TIMER_CH1:
            TIMx->CCMR1 &= ~(0x7 << 4);   /* Clear mode */
            TIMx->CCMR1 |=  (0x6 << 4);   /* PWM Mode 1 */
            SET_BIT(TIMx->CCMR1, 3);      /* Preload Enable */
            SET_BIT(TIMx->CCER, 0);       /* Output Enable */
            TIMx->CCR1 = 0;               /* Start at 0 */
            break;
        case TIMER_CH2:
            TIMx->CCMR1 &= ~(0x7 << 12);  
            TIMx->CCMR1 |=  (0x6 << 12);  
            SET_BIT(TIMx->CCMR1, 11);     
            SET_BIT(TIMx->CCER, 4);       
            TIMx->CCR2 = 0;
            break;
        case TIMER_CH3:
            TIMx->CCMR2 &= ~(0x7 << 4);   
            TIMx->CCMR2 |=  (0x6 << 4);   
            SET_BIT(TIMx->CCMR2, 3);      
            SET_BIT(TIMx->CCER, 8);       
            TIMx->CCR3 = 0;
            break;
        case TIMER_CH4:
            TIMx->CCMR2 &= ~(0x7 << 12);  
            TIMx->CCMR2 |=  (0x6 << 12);  
            SET_BIT(TIMx->CCMR2, 11);     
            SET_BIT(TIMx->CCER, 12);      
            TIMx->CCR4 = 0;
            break;
    }

    /* 4. Start the counter */
    SET_BIT(TIMx->CR1, 0);
}

void Timer_SetPWM_Duty(timer* TIMx, uint8 Channel, uint32 Duty_Value) {
    /* Safety check against ARR is done at the HAL level now */
    switch(Channel) {
        case TIMER_CH1: TIMx->CCR1 = Duty_Value; break;
        case TIMER_CH2: TIMx->CCR2 = Duty_Value; break;
        case TIMER_CH3: TIMx->CCR3 = Duty_Value; break;
        case TIMER_CH4: TIMx->CCR4 = Duty_Value; break;
    }
}

/* SysTick Delays remain exactly the same */
void Timer_Delay_us(uint32 us) {
    SYSTICK_LOAD = (16 * us) - 1; 
    SYSTICK_VAL = 0;                  
    SYSTICK_CTRL = 5;                 
    while ((SYSTICK_CTRL & (1 << 16)) == 0); 
    SYSTICK_CTRL = 0;                 
}

void Timer_Delay_ms(uint32 ms) {
    for (uint32 i = 0; i < ms; i++) {
        Timer_Delay_us(1000);
    }
}