#include "temp.h"
#include "../../mcal/adc/adc.h"

volatile uint16 Temp_Buffer = 0;

void TEMP_Init(uint8 Channel) {
    /* 1. Initialize the ADC logic in continuous conversion mode */
    ADC_Init(Channel, ADC_RESOLUTION_12B, ADC_CONTINUOUS);

    /* 2. Enable ADC Interrupts (replaces DMA) */
    ADC_EnableInterrupt();

    /* 3. Trigger the very first conversion */
    ADC_Start();
}

uint16 TEMP_GetCelsius_x10(void) {
    /* Get the raw data the IRQ updated invisibly */
    uint16 raw_digital = Temp_Buffer; 
    
    /* Convert digital value to physical voltage (millivolts) */
    uint32 milliVolts = ((uint32)raw_digital * TEMP_VOLTAGE) / TEMP_RESOLUTION;
    
    /* Convert voltage to Temperature. */
    uint16 temp_x10 = (uint16)milliVolts;
    
    /* Removed ADC_Start() for continuous mode */

    return temp_x10;
}