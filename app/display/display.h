#ifndef DISPLAY_H
#define DISPLAY_H

#include "../../core/STD_TYPES.h"

void DISPLAY_Init(void);

void DISPLAY_Update(uint16 temp_x10, uint8 fan_speed, uint8 is_overheat);

#endif