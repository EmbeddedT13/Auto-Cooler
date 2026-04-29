#ifndef COOLER_SM_H
#define COOLER_SM_H

#include "../../core/STD_TYPES.h"


typedef enum{

    COOLER_IDLE,
    COOLER_LOW,
    COOLER_MID,
    COOLER_HIGH,
    COOLER_ALARM
}CoolerState_t;

#define THRESH_TEMP_LOW    270  /* 27.0 °C */
#define THRESH_TEMP_MID    320  /* 32.0 °C */
#define THRESH_TEMP_MAX    370  /* 37.0 °C */
#define THRESH_TEMP_ALARM  420  /* 42.0 °C */


void COOLER_Init(void);
void COOLER_Update(void);


#endif