#include "gpio.h"

void GPIO_SetPinMode(gpio *GPIOx, uint8 PinNum, uint8 Mode){
    if(PinNum <= 15){
        WRITE_BIT_FIELD(GPIOx->MODER, 0x03, PinNum * 2, Mode);
    }
}

void GPIO_SetPullType(gpio *GPIOx, uint8 PinNum, uint8 PullType){
        if(PinNum <= 15){
        WRITE_BIT_FIELD(GPIOx->PUPDR, 0x03, PinNum * 2, PullType);
    }
}

void GPIO_WritePin(gpio *GPIOx, uint8 PinNum, uint8 State){
    if(PinNum <= 15){
        if(State == GPIO_STATE_HIGH) {
            GPIOx->BSRR = 1 << PinNum;
        } else {
            GPIOx->BSRR = 1 << (PinNum + 16);
        }
    }
}

uint8 GPIO_ReadPin(gpio *GPIOx, uint8 PinNum){
    uint8  bit_stat = 0;

    if(PinNum <= 15){
        bit_stat = READ_BIT(GPIOx->IDR, PinNum);
    }

    return bit_stat;
}

void GPIO_WriteGroup(gpio *GPIOx, uint16 Mask, uint16 Value)
{
    uint32 set_bits = Value & Mask;
    
    uint32 reset_bits = ((~Value) & Mask) << 16;
    GPIOx->BSRR = set_bits | reset_bits;
}

void GPIO_SetAlternateFunction(gpio *GPIOx, uint8 PinNum, uint8 AF_Value) {
    if (PinNum < 8) {
        GPIOx->AFRL &= ~(0xF << (PinNum * 4));        
        GPIOx->AFRL |=  (AF_Value << (PinNum * 4));   
    } else {
        GPIOx->AFRH &= ~(0xF << ((PinNum - 8) * 4));      
        GPIOx->AFRH |=  (AF_Value << ((PinNum - 8) * 4));
    }
}