#include "adc.h"
#include "../../core/BIT_MATH.h"
#include "../gpio/gpio.h"
#include "../rcc/rcc.h"
#include "../dma/dma.h"

void ADC_Init(uint8 Channel, uint8 Res, uint8 Mode){

    RCC_EnableClock(RCC_APB2, RCC_APB2_ADC1_BIT);

    #define ADC_CCR (*((volatile uint32*)0x40012304))
    ADC_CCR &= ~(0x3 << 16);
    ADC_CCR |=  (0x1 << 16);

    if(Channel >= 0 && Channel <= 7){
        RCC_EnableClock(RCC_AHB1, GPIOA_AHB1_BIT);
        GPIO_SetPinMode(GPIOA, Channel, GPIO_MODE_ANALOG);
        GPIO_SetPullType(GPIOA, Channel, GPIO_PR_NONE);
        WRITE_BIT_FIELD(ADC->ADC_SMPR2, 0x07, (Channel*3), ADC_SAMPLE_TIME_480);
    }
    else if(Channel >= 8 && Channel <= 9){
        RCC_EnableClock(RCC_AHB1, GPIOB_AHB1_BIT);
        /* FIX: ADC_IN8=PB0, ADC_IN9=PB1 — subtract 8 for pin number */
        GPIO_SetPinMode(GPIOB, Channel - 8, GPIO_MODE_ANALOG);
        GPIO_SetPullType(GPIOB, Channel - 8, GPIO_PR_NONE);
        WRITE_BIT_FIELD(ADC->ADC_SMPR2, 0x07, (Channel*3), ADC_SAMPLE_TIME_480);
    }
    else if(Channel >= 10 && Channel <= 15){
        RCC_EnableClock(RCC_AHB1, GPIOC_AHB1_BIT);
        /* FIX: ADC_IN10=PC0 ... ADC_IN15=PC5 — subtract 10 for pin number */
        GPIO_SetPinMode(GPIOC, Channel - 10, GPIO_MODE_ANALOG);
        GPIO_SetPullType(GPIOC, Channel - 10, GPIO_PR_NONE);
        WRITE_BIT_FIELD(ADC->ADC_SMPR1, 0x07, ((Channel - 10) * 3), ADC_SAMPLE_TIME_480);
    }

    WRITE_BIT_FIELD(ADC->ADC_CR1, 0x03, 24, Res);

    /* FIX: Always use DISCONTINUOUS for interrupt-driven mode.
     * CONTINUOUS + EOCIE = ISR storm that starves the main loop.
     * We retrigger manually from the ISR after the main loop consumes data. */
    CLEAR_BIT(ADC->ADC_CR2, 1); /* CONT = 0: single conversion per trigger */

    CLEAR_BIT(ADC->ADC_CR2, 11); /* Right alignment */

    WRITE_BIT_FIELD(ADC->ADC_SQR1, 0x0F, 20, 0);      /* Sequence length = 1 */
    WRITE_BIT_FIELD(ADC->ADC_SQR3, 0x1F, 0, Channel); /* Channel in slot 1 */
}

void ADC_Start(void){
    /* EOCS=1: EOC fires after each single conversion (correct for single-channel) */
    SET_BIT(ADC->ADC_CR2, 10);

    if (READ_BIT(ADC->ADC_CR2, 0) == 0) {
        SET_BIT(ADC->ADC_CR2, 0);   /* ADON */
        volatile uint32 delay = 10000; while(delay--);
    }

    SET_BIT(ADC->ADC_CR2, 30);  /* SWSTART: trigger first conversion */
}

/* Shared between adc.c and temp.c */
volatile uint16 Temp_Buffer = 0;
volatile uint8  Temp_DataReady = 0; /* NEW: flag set by ISR, cleared by reader */

void ADC_EnableInterrupt(void) {
    SET_BIT(ADC->ADC_CR1, 5); /* EOCIE */

    #define NVIC_ISER0 (*((volatile uint32*)0xE000E100))
    SET_BIT(NVIC_ISER0, 18);
}

void ADC_IRQHandler(void) {
    if (READ_BIT(ADC->ADC_SR, 1)) { /* EOC set? */

        Temp_Buffer   = (uint16)(ADC->ADC_DR & 0x0FFF); /* Read clears EOC */
        Temp_DataReady = 1; /* Signal main loop: fresh data available */

        /* FIX: Do NOT retrigger here. Let the main loop consume the value
         * first, then call ADC_Start() again. This prevents ISR starvation.
         * SWSTART will be called from TEMP_GetCelsius_x10(). */

        CLEAR_BIT(ADC->ADC_SR, 1); /* Proteus quirk: force-clear EOC */
    }
}

void ADC_TriggerNext(void) {
    /* Called by temp layer after consuming Temp_Buffer */
    SET_BIT(ADC->ADC_CR2, 30); /* SWSTART */
}

void ADC_Start_DMA(uint16* DestinationBuffer) {
    RCC_EnableClock(RCC_AHB1, RCC_AHB1_DMA2_BIT);

    DMA_Config_t dma_conf;
    dma_conf.Channel      = DMA_CHANNEL_0;
    dma_conf.Direction    = DMA_DIRECTION_PER_TO_MEM;
    dma_conf.MemDataSize  = DMA_MEM_DATA_SIZE_HALF_WORD;
    dma_conf.PerDataSize  = DMA_PER_DATA_SIZE_HALF_WORD;
    dma_conf.MemIncrement = DMA_MEM_INCREMENT_DISABLE;
    dma_conf.PerIncrement = DMA_PER_INCREMENT_DISABLE;
    dma_conf.CircularMode = DMA_CIRCULAR_ENABLE;

    DMA_Init(&DMA2->STREAM[0], &dma_conf);
    DMA_SetAddresses(&DMA2->STREAM[0], (uint32)&ADC->ADC_DR,
                     (uint32)DestinationBuffer, 1);
    DMA_Enable(&DMA2->STREAM[0]);

    SET_BIT(ADC->ADC_CR2, 0);
    volatile uint32 delay = 1000; while(delay--);
    SET_BIT(ADC->ADC_CR2, 8);
    SET_BIT(ADC->ADC_CR2, 9);
    SET_BIT(ADC->ADC_CR2, 30);
}