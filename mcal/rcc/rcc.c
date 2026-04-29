#include "rcc.h"


void RCC_EnableClock(uint8 BusId, uint8 PeripheralBit){

    /* Route the peripheral bit to the corresponding bus enable register */
    switch(BusId){

        case RCC_APB1:
        SET_BIT(RCC -> RCC_APB1ENR, PeripheralBit);
        break;

        case RCC_APB2:
        SET_BIT(RCC -> RCC_APB2ENR, PeripheralBit);
        break;

        case RCC_AHB1:
        SET_BIT(RCC -> RCC_AHB1ENR, PeripheralBit);
        break;
    }
}

void RCC_EnableTIM3(void) {
    SET_BIT(RCC -> RCC_APB1ENR, RCC_APB1_TIM3_BIT);
}

void RCC_EnableADC1(void) {
    SET_BIT(RCC -> RCC_APB2ENR, RCC_APB2_ADC1_BIT);
}

void RCC_EnableDMA2(void) {
    SET_BIT(RCC -> RCC_AHB1ENR, RCC_AHB1_DMA2_BIT);
}

void RCC_EnableTIM2(void) {
    SET_BIT(RCC -> RCC_APB1ENR, 0); /* Bit 0 is TIM2EN */
}

void RCC_EnableTIM4(void) {
    SET_BIT(RCC -> RCC_APB1ENR, 2); /* Bit 2 is TIM4EN */
}