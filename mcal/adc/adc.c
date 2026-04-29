#include "adc.h"
#include "../../core/BIT_MATH.h"
#include "../gpio/gpio.h"
#include "../rcc/rcc.h"
#include "../dma/dma.h"

void ADC_Init(uint8 Channel, uint8 Res, uint8 Mode){

    /* Enable clock for ADC */
    RCC_EnableClock(RCC_APB2, RCC_APB2_ADC1_BIT);

    /* Proteus 8.13 quirk: The ADC Common Control Register (ADC_CCR at 0x40012304) 
     * must have its prescaler explicitly set. We set it to ADCPRE = 01 (PCLK2/4). 
     * If left at reset, the simulator treats the clock as 0 and conversions never happen. */
    #define ADC_CCR (*((volatile uint32*)0x40012304))
    ADC_CCR &= ~(0x3 << 16);
    ADC_CCR |=  (0x1 << 16);

    /* Route the channel to the correct GPIO port and set Sample Time */
    if(Channel >= 0 && Channel <= 7){
        RCC_EnableClock(RCC_AHB1, GPIOA_AHB1_BIT);
        GPIO_SetPinMode(GPIOA, Channel, GPIO_MODE_ANALOG);
        GPIO_SetPullType(GPIOA, Channel, GPIO_PR_NONE);
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
    /* Proteus Quirks: Ensure EOCS is set so EOC fires properly for single conversions */
    SET_BIT(ADC->ADC_CR2, 10); 

    if (READ_BIT(ADC->ADC_CR2, 0) == 0) {
        SET_BIT(ADC->ADC_CR2, 0);   /* ADON: power on the ADC */
        /* t_stab: wait for ADC to stabilise before SWSTART */
        volatile uint32 delay = 10000; while(delay--);
    }
    
    SET_BIT(ADC->ADC_CR2, 30);  /* SWSTART: trigger conversion */
}

extern volatile uint16 Temp_Buffer;

void ADC_EnableInterrupt(void) {
    /* Enable EOCIE (End of Conversion Interrupt Enable) in CR1 */
    SET_BIT(ADC->ADC_CR1, 5);

    /* Enable NVIC IRQ 18 for ADC (STM32F401) */
    #define NVIC_ISER0 (*((volatile uint32*)0xE000E100))
    SET_BIT(NVIC_ISER0, 18);
}

void ADC_IRQHandler(void) {
    /* Check if EOC flag is set */
    if (READ_BIT(ADC->ADC_SR, 1)) {
        /* Reading DR automatically clears the EOC flag in hardware */
        Temp_Buffer = (uint16)(ADC->ADC_DR & 0x0FFF);
        
        /* Proteus Quirk: Explicitly clear the EOC bit just in case */
        CLEAR_BIT(ADC->ADC_SR, 1);
    }
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

    /* 3. Tell ADC to use DMA and start — order matters (RM0368 §11.3.3):
     *   a) ADON first so the ADC powers up
     *   b) t_stab delay — SWSTART must NOT fire until ADC is stable
     *   c) DMA/DDS bits — enable DMA handshake
     *   d) SWSTART — now safe to trigger the first conversion */
    SET_BIT(ADC->ADC_CR2, 0);   /* a) ADON */
    volatile uint32 delay = 1000; while(delay--); /* b) t_stab */
    SET_BIT(ADC->ADC_CR2, 8);   /* c) DMA enable */
    SET_BIT(ADC->ADC_CR2, 9);   /* c) DDS */
    SET_BIT(ADC->ADC_CR2, 30);  /* d) SWSTART */
}