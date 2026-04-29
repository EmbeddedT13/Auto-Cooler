#ifndef ADC_H
#define ADC_H

#include "../../core/STD_TYPES.h"

typedef struct {
    volatile uint32 ADC_SR ;
    volatile uint32 ADC_CR1;
    volatile uint32 ADC_CR2;
    volatile uint32 ADC_SMPR1;
    volatile uint32 ADC_SMPR2;
    volatile uint32 ADC_JOFR1;
    volatile uint32 ADC_JOFR2;
    volatile uint32 ADC_JOFR3;
    volatile uint32 ADC_JOFR4;
    volatile uint32 ADC_HTR;
    volatile uint32 ADC_LTR;
    volatile uint32 ADC_SQR1;
    volatile uint32 ADC_SQR2;
    volatile uint32 ADC_SQR3;
    volatile uint32 ADC_JSQR;
    volatile uint32 ADC_JDR1;
    volatile uint32 ADC_JDR2;
    volatile uint32 ADC_JDR3;
    volatile uint32 ADC_JDR4;
    volatile uint32 ADC_DR;
}adc_t;

#define ADC_BASE_ADDRESS 0x40012000
#define ADC ((adc_t *) ADC_BASE_ADDRESS)

/* ADC Channels (Mapped to GPIO pins A0-A7, B0-B1, C0-C5) */
#define ADC_CHANNEL_0   0
#define ADC_CHANNEL_1   1
#define ADC_CHANNEL_2   2
#define ADC_CHANNEL_3   3
#define ADC_CHANNEL_4   4
#define ADC_CHANNEL_5   5
#define ADC_CHANNEL_6   6
#define ADC_CHANNEL_7   7
#define ADC_CHANNEL_8   8
#define ADC_CHANNEL_9   9
#define ADC_CHANNEL_10  10
#define ADC_CHANNEL_11  11
#define ADC_CHANNEL_12  12
#define ADC_CHANNEL_13  13
#define ADC_CHANNEL_14  14
#define ADC_CHANNEL_15  15

/* ADC Sample Time Options (in Clock Cycles) */
#define ADC_SAMPLE_TIME_3      0
#define ADC_SAMPLE_TIME_15     1
#define ADC_SAMPLE_TIME_28     2
#define ADC_SAMPLE_TIME_56     3
#define ADC_SAMPLE_TIME_84     4
#define ADC_SAMPLE_TIME_112    5
#define ADC_SAMPLE_TIME_144    6
#define ADC_SAMPLE_TIME_480    7

/* ADC Resolution Options */
#define ADC_RESOLUTION_12B 0
#define ADC_RESOLUTION_10B 1
#define ADC_RESOLUTION_8B  2
#define ADC_RESOLUTION_6B  3

/* ADC Modes */
#define ADC_DISCONTINUOUS 0
#define ADC_CONTINUOUS    1

#define ADC_IRQ 18

void ADC_Init(uint8 Channel, uint8 Res, uint8 Mode);
void ADC_Start(void);
void ADC_EnableInterrupt(void);
void ADC_Start_DMA(uint16* DestinationBuffer);
#endif