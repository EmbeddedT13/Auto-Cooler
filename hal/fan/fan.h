#ifndef FAN_H
#define FAN_H

#include "../../core/STD_TYPES.h"

/* Fan Speed Percentages (0 to 100) */
#define FAN_SPEED_OFF  0
#define FAN_SPEED_LOW  33
#define FAN_SPEED_MED  66
#define FAN_SPEED_MAX  100

void FAN_Init(void);
void FAN_SetSpeed(uint8 SpeedPercent);

#endif