#include "adc.h"
#include "../../core/BIT_MATH.h"
#include "../gpio/gpio.h"
#include "../rcc/rcc.h"
#include "../dma/dma.h"

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
    WRITE_BIT_FIELD(ADC->ADC_SQR3, 0x1F, 0, Channel); /* Assign channel to sequence */

}

void ADC_Start(void){
    SET_BIT(ADC->ADC_CR2, 0);  /* Power on the ADC */    
    SET_BIT(ADC->ADC_CR2, 30); /* Trigger the first continuous conversion */
}

void ADC_Start_DMA(uint16* DestinationBuffer) {
    /* 1. Turn on the DMA2 Clock */
    RCC_EnableClock(RCC_AHB1, RCC_AHB1_DMA2_BIT);

    /* 2. Configure the DMA Stream for ADC1 */
    DMA_Config_t dma_conf;
    dma_conf.Channel = DMA_CHANNEL_0;
    dma_conf.Direction = DMA_DIRECTION_PER_TO_MEM;
    dma_conf.MemDataSize = DMA_MEM_DATA_SIZE_HALF_WORD; 
    dma_conf.PerDataSize = DMA_PER_DATA_SIZE_HALF_WORD; 
    dma_conf.MemIncrement = DMA_MEM_INCREMENT_DISABLE; 
    dma_conf.PerIncrement = DMA_PER_INCREMENT_DISABLE;
    dma_conf.CircularMode = DMA_CIRCULAR_ENABLE; 

    DMA_Init(&DMA2->STREAM[0], &dma_conf);
    DMA_SetAddresses(&DMA2->STREAM[0], (uint32)&ADC->ADC_DR, (uint32)DestinationBuffer, 1);
    DMA_Enable(&DMA2->STREAM[0]);

    /* 3. Tell ADC to use DMA and start */
    SET_BIT(ADC->ADC_CR2, 8); 
    SET_BIT(ADC->ADC_CR2, 9); 
    
    SET_BIT(ADC->ADC_CR2, 0);  
    SET_BIT(ADC->ADC_CR2, 30); 
}