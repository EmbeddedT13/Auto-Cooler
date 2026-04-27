#include "adc.h"
#include "../../core/BIT_MATH.h"
#include "../gpio/gpio.h"
#include "../nvic/nvic.h"
#include "../rcc/rcc.h"




void ADC_Init(uint8 Channel, uint8 Res, uint8 Mode){

    /* Enable clock for ADC */
    RCC_EnableClock(RCC_APB2, RCC_APB2_ADC1_BIT);

    /* Route the channel to the correct GPIO port and set Sample Time */
    if(Channel >= 0 && Channel <= 7){
        RCC_EnableClock(RCC_AHB1, GPIOA_AHB1_BIT);
        GPIO_SetPinMode(GPIOA, Channel, GPIO_MODE_ANALOG);
        WRITE_BIT_FIELD(ADC->ADC_SMPR2, 0x07, (Channel*3), ADC_SAMPLE_TIME_480);
    }
    else if(Channel >= 8 && Channel <= 9){
        RCC_EnableClock(RCC_AHB1, GPIOB_AHB1_BIT);
        GPIO_SetPinMode(GPIOB, Channel, GPIO_MODE_ANALOG);
        WRITE_BIT_FIELD(ADC->ADC_SMPR2, 0x07, (Channel*3), ADC_SAMPLE_TIME_480);
    }
    else if(Channel >= 10 && Channel <= 15){
        RCC_EnableClock(RCC_AHB1, GPIOC_AHB1_BIT);
        GPIO_SetPinMode(GPIOC, Channel, GPIO_MODE_ANALOG);
        WRITE_BIT_FIELD(ADC->ADC_SMPR1, 0x07, ((Channel - 10) * 3), ADC_SAMPLE_TIME_480);
    
    }

    WRITE_BIT_FIELD(ADC->ADC_CR1, 0x03, 24, Res); /*Clear res bits in CR1 and put Res value in these bits*/
        
    if(Mode == ADC_CONTINUOUS){
        SET_BIT(ADC->ADC_CR2, 1); /* Enable Continuous Conversion */
    }
    if(Mode == ADC_DISCONTINUOUS){
        CLEAR_BIT(ADC->ADC_CR2, 1); /* Enable Discontinuous Conversion */
    }

    CLEAR_BIT(ADC->ADC_CR2, 11); /*Making sure of right allignment*/
    
    WRITE_BIT_FIELD(ADC->ADC_SQR1, 0x0F, 20, 0); /* Set Sequence length to 1 */
    
    SET_BIT(ADC->ADC_CR2, 0);  /* Power on the ADC */    

}

uint16 ADC_ReadChannel(uint8 Channel){
    WRITE_BIT_FIELD(ADC->ADC_SQR3, 0x1F, 0, Channel);

    SET_BIT(ADC->ADC_CR2, 30); 

    while(READ_BIT(ADC->ADC_SR, 1) == 0) {
        /* CPU blocks here for a few microseconds until the reading is ready */
    }
    return ADC->ADC_DR; 
}

