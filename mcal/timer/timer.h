#ifndef TIMER_H
#define TIMER_H

#include "../../core/STD_TYPES.h"
#include "../../core/BIT_MATH.h"

typedef struct
{
    volatile uint32 CR1;       /* Offset: 0x00 */
    volatile uint32 CR2;       /* Offset: 0x04 */
    volatile uint32 SMCR;      /* Offset: 0x08 */
    volatile uint32 DIER;      /* Offset: 0x0C */
    volatile uint32 SR;        /* Offset: 0x10 */
    volatile uint32 EGR;       /* Offset: 0x14 */
    volatile uint32 CCMR1;     /* Offset: 0x18 */
    volatile uint32 CCMR2;     /* Offset: 0x1C */
    volatile uint32 CCER;      /* Offset: 0x20 */
    volatile uint32 CNT;       /* Offset: 0x24 */
    volatile uint32 PSC;       /* Offset: 0x28 */
    volatile uint32 ARR;       /* Offset: 0x2C */
    volatile uint32 Reserved;  /* Offset: 0x30 */
    volatile uint32 CCR1;      /* Offset: 0x34 */
    volatile uint32 CCR2;      /* Offset: 0x38 */
    volatile uint32 CCR3;      /* Offset: 0x3C */
    volatile uint32 CCR4;      /* Offset: 0x40 */
}timer;

/* Base Address */
#define TIM3_BASE_ADD 0x40000400
#define TIM3 ((timer*) TIM3_BASE_ADD)

/* Public APIs */
void Timer3_PWM_Init(void);
void Timer3_SetPWM_DutyCycle(uint8 duty_percent);

#endif