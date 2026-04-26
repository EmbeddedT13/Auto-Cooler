#include "adc.h"
#include "../gpio/gpio.h"
#include "../nvic/nvic.h"
#include "../rcc/rcc.h"


volatile uint16 ADC_ResultBuffer = 0;


void ADC_Init(uint8 Channel, uint8 Res, uint8 Mode){

    /* Enable clock for ADC */
    RCC_EnableClock(RCC_APB2, 8);

    if(Channel >= 0 && Channel <= 7){
        
    }
    else if(Channel >= 8 && Channel <= 9){
        
    }
    else if(Channel >= 10 && Channel <= 15){
        
    }
    

}

void ADC_Start(void){

}

uint16 ADC_Read(void){

}