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