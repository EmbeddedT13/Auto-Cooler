#ifndef TEMP_H
#define TEMP_H

#include "../../core/STD_TYPES.h"
#include "../../mcal/adc/adc.h"

#define TEMP_SENSOR_CHANNEL ADC_CHANNEL_0
#define TEMP_VOLTAGE     3300  /*Vref = 3300 mv*/
#define TEMP_RESOLUTION  4096  /*ADC resolution = 2^12 = 4096 steps*/


void TEMP_Init(uint8 Channel);
uint16 TEMP_GetCelsius_x10(void);

#endif