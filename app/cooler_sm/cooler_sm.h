#ifndef COOLER_SM_H
#define COOLER_SM_H

#include "../../core/STD_TYPES.h"


typedef enum {
    COOLER_IDLE,      
    COOLER_COOLING,   
    COOLER_OVERHEAT   
} CoolerState_t;

#define THRESH_TEMP_LOW    250  /* 25.0 °C */
#define THRESH_TEMP_MED    300  /* 30.0 °C */
#define THRESH_TEMP_MAX    350  /* 35.0 °C */
#define THRESH_TEMP_ALARM  400  /* 40.0 °C */


void COOLER_Init(void);
void COOLER_Update(void);


#endif