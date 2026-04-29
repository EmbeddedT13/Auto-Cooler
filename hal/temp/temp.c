#include "temp.h"
#include "../../mcal/adc/adc.h"

volatile uint16 Temp_Buffer = 0;

void TEMP_Init(uint8 Channel) {
    /* 1. Initialize the ADC logic */
    ADC_Init(Channel, ADC_RESOLUTION_12B, ADC_CONTINUOUS);

    /* 2. Tell the MCAL to handle the heavy lifting and pipe data to our buffer */
    ADC_Start_DMA((uint16*)&Temp_Buffer);
}

uint16 TEMP_GetCelsius_x10(void) {
    /* Get the raw data the DMA updated invisibly */
    uint16 raw_digital = Temp_Buffer; 
    
    /* Convert digital value to physical voltage (millivolts) */
    uint32 milliVolts = ((uint32)raw_digital * TEMP_VOLTAGE) / TEMP_RESOLUTION;
    
    /* Convert voltage to Temperature. */
    uint16 temp_x10 = (uint16)milliVolts;
    
    return temp_x10;
}