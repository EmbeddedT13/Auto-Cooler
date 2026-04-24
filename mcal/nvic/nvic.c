#include "nvic.h"


void NVIC_EnableInterrupt(uint8 IRQ_Num){

    if(IRQ_Num <= 31){

        NVIC_ISER0 = (1 << IRQ_Num);
    }
    else if (IRQ_Num <= 63){

        NVIC_ISER1 = (1 << (IRQ_Num - 32));
    }
}

void NVIC_DisableInterrupt(uint8 IRQ_Num){

    if(IRQ_Num <= 31){

        NVIC_ICER0 = (1 << IRQ_Num);
    }
    else if (IRQ_Num <= 63){

        NVIC_ICER1 = (1 << (IRQ_Num - 32));
    }

}

void NVIC_SetPriority(uint8 IRQ_Num, uint8 Priority){

    NVIC_IPR[IRQ_Num] = (Priority << 4);
}
