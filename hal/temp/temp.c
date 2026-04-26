#include "temp.h"
#include "../../mcal/adc/adc.h"


void TEMP_Init(uint8 Channel){

    ADC_Init(Channel, ADC_RESOLUTION_12B, ADC_CONTINUOUS); /* Initialize ADC for the requested channel, 12-bit resolution, Continuous Mode */

    ADC_Start(); /* Power on the ADC and trigger the first conversion */
}


uint16 TEMP_GetCelsius_x10(void){

    uint16 adc_value = ADC_Read(); /* Fetch the latest autonomous 12-bit reading */

    uint16 temp_x10 = (uint16) (((uint32) adc_value * TEMP_VOLTAGE) / TEMP_RESOLUTION); /* Calculate Temperature x 10 using fixed-point math to avoid floats */

    return temp_x10;
}