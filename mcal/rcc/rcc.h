#ifndef RCC_H
#define RCC_H

#include "../../core/BIT_MATH.h"
#include "../../core/STD_TYPES.h"

/*Base address for RCC*/
#define RCC_BASE_ADDRESS 0x40023800 

/* RCC Peripheral Register Map */
typedef struct 
{ 
    volatile uint32 RCC_CR;        /*Address offset: 0x00*/
    volatile uint32 RCC_PLLCFGR;   /*Address offset: 0x04*/
    volatile uint32 RCC_CFGR;      /*Address offset: 0x08*/
    volatile uint32 RCC_CIR;       /*Address offset: 0x0C*/
    volatile uint32 RCC_AHB1RSTR;  /*Address offset: 0x10*/
    volatile uint32 RCC_AHB2RSTR;  /*Address offset: 0x14*/
    volatile uint32 Reserved1;     /*Address offset: 0x18*/
    volatile uint32 Reserved2;     /*Address offset: 0x1C*/
    volatile uint32 RCC_APB1RSTR;  /*Address offset: 0x20*/
    volatile uint32 RCC_APB2RSTR;  /*Address offset: 0x24*/
    volatile uint32 Reserved3;     /*Address offset: 0x28*/
    volatile uint32 Reserved4;     /*Address offset: 0x2C*/
    volatile uint32 RCC_AHB1ENR;   /*Address offset: 0x30*/
    volatile uint32 RCC_AHB2ENR;   /*Address offset: 0x34*/
    volatile uint32 Reserved5;     /*Address offset: 0x38*/
    volatile uint32 Reserved6;     /*Address offset: 0x3C*/
    volatile uint32 RCC_APB1ENR;   /*Address offset: 0x40*/
    volatile uint32 RCC_APB2ENR;   /*Address offset: 0x44*/
}rcc;

#define RCC ((volatile rcc*) RCC_BASE_ADDRESS)

/* Bus IDs */
#define RCC_AHB1 0
#define RCC_APB1 1
#define RCC_APB2 2

/* Peripheral Clock Enable Bits */
#define GPIOA_AHB1_BIT      0
#define GPIOB_AHB1_BIT      1
#define GPIOC_AHB1_BIT      2
#define SYSCFG_APB2_BIT     14
#define RCC_APB1_TIM3_BIT   1
#define RCC_APB2_ADC1_BIT   8
#define RCC_AHB1_DMA2_BIT   22

void RCC_EnableClock(uint8 BusId, uint8 PeripheralBit);
void RCC_EnableTIM3(void);
void RCC_EnableADC1(void);
void RCC_EnableDMA2(void);


#endif
