#ifndef LED_H
#define LED_H

#include "../../core/STD_TYPES.h"
#include "../../mcal/gpio/gpio.h"

/* LED Port Definition */
#define LED_PORT  GPIOC

/* LED Pin Definitions based on Proteus Schematic */
#define LED_SUCCESS GPIO_PIN00  /* Green LED is on PC0 */
#define LED_PROG_1  GPIO_PIN01  /* Yellow LED is on PC1 */
#define LED_ALARM   GPIO_PIN02  /* Red LED is on PC2 */
#define LED_ERROR   GPIO_PIN03      /* Blue   (PC3) - Sensor Disconnected! */
#define LED_PROG_2  GPIO_PIN03
#define LED_PROG_3  GPIO_PIN04
#define LED_PROG_4  GPIO_PIN05
#define LED_DOORBELL GPIO_PIN06

/* Function Prototypes */
void LED_Init(uint8 LedPin);
void LED_On(uint8 LedPin);
void LED_Off(uint8 LedPin);
void LED_Toggle(uint8 LedPin);

#endif
