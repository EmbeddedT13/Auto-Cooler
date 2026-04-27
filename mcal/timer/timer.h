#ifndef TIMER_H
#define TIMER_H

#include "../../core/STD_TYPES.h"
#include "../../core/BIT_MATH.h"

/* --- General Timer Register Map --- */
typedef struct {
    volatile uint32 CR1;       /* 0x00 */
    volatile uint32 CR2;       /* 0x04 */
    volatile uint32 SMCR;      /* 0x08 */
    volatile uint32 DIER;      /* 0x0C */
    volatile uint32 SR;        /* 0x10 */
    volatile uint32 EGR;       /* 0x14 */
    volatile uint32 CCMR1;     /* 0x18 */
    volatile uint32 CCMR2;     /* 0x1C */
    volatile uint32 CCER;      /* 0x20 */
    volatile uint32 CNT;       /* 0x24 */
    volatile uint32 PSC;       /* 0x28 */
    volatile uint32 ARR;       /* 0x2C */
    volatile uint32 Reserved;  /* 0x30 */
    volatile uint32 CCR1;      /* 0x34 */
    volatile uint32 CCR2;      /* 0x38 */
    volatile uint32 CCR3;      /* 0x3C */
    volatile uint32 CCR4;      /* 0x40 */
} timer;

/* Hardware Base Addresses */
#define TIM2 ((timer*) 0x40000000)
#define TIM3 ((timer*) 0x40000400)
#define TIM4 ((timer*) 0x40000800)
#define TIM5 ((timer*) 0x40000C00)

/* Channels */
#define TIMER_CH1 1
#define TIMER_CH2 2
#define TIMER_CH3 3
#define TIMER_CH4 4

/* SysTick Registers */
#define SYSTICK_CTRL  (*((volatile uint32*)0xE000E010))
#define SYSTICK_LOAD  (*((volatile uint32*)0xE000E014))
#define SYSTICK_VAL   (*((volatile uint32*)0xE000E018))

/* --- Public APIs --- */
void Timer_PWM_Init(timer* TIMx, uint8 Channel, uint16 PSC_Value, uint16 ARR_Value);
void Timer_SetPWM_Duty(timer* TIMx, uint8 Channel, uint32 Duty_Value);

void Timer_Delay_ms(uint32 ms);
void Timer_Delay_us(uint32 us);

#endif