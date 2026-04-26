#ifndef GPIO_H
#define GPIO_H

#include "../../core/BIT_MATH.h"
#include "../../core/STD_TYPES.h"


/*Peripheral Struct*/
typedef struct
{
    volatile uint32 MODER;  /*Address offset: 0x00 */
    volatile uint32 OTYPER; /*Address offset: 0x04 */
    volatile uint32 OSPEEDR;/*Address offset: 0x08 */
    volatile uint32 PUPDR;  /*Address offset: 0x0C */
    volatile uint32 IDR;    /*Address offset: 0x10 */
    volatile uint32 ODR;    /*Address offset: 0x14 */
    volatile uint32 BSRR;   /*Address offset: 0x18 */

} gpio;

/*Base Addresses*/
#define GPIOA_BaseAddress 0x40020000
#define GPIOB_BaseAddress 0x40020400
#define GPIOC_BaseAddress 0x40020800
#define GPIOD_BaseAddress 0x40020C00
#define GPIOE_BaseAddress 0x40021000

/*Peripheral Pointers*/
#define GPIOA ((gpio *) GPIOA_BaseAddress)
#define GPIOB ((gpio *) GPIOB_BaseAddress)
#define GPIOC ((gpio *) GPIOC_BaseAddress)
#define GPIOD ((gpio *) GPIOD_BaseAddress)
#define GPIOE ((gpio *) GPIOE_BaseAddress)

/*API Definitions*/

/*PINS*/
#define GPIO_PIN00 0
#define GPIO_PIN01 1
#define GPIO_PIN02 2
#define GPIO_PIN03 3
#define GPIO_PIN04 4
#define GPIO_PIN05 5
#define GPIO_PIN06 6
#define GPIO_PIN07 7
#define GPIO_PIN08 8
#define GPIO_PIN09 9
#define GPIO_PIN10 10
#define GPIO_PIN11 11
#define GPIO_PIN12 12
#define GPIO_PIN13 13
#define GPIO_PIN14 14
#define GPIO_PIN15 15


/*MODES*/
#define GPIO_MODE_INPUT     0x00
#define GPIO_MODE_OUTPUT    0x01
#define GPIO_MODE_ALTERNATE 0x02
#define GPIO_MODE_ANALOG    0x03

/*PULL RESISTORS*/
#define GPIO_PR_NONE        0x00
#define GPIO_PR_PU          0x01

/*STATES*/
#define GPIO_STATE_LOW      0x00
#define GPIO_STATE_HIGH     0x01


void GPIO_SetPinMode(gpio *GPIOx, uint8 PinNum, uint8 Mode);
void GPIO_SetPullType(gpio *GPIOx, uint8 PinNum, uint8 PullType);
void GPIO_WritePin(gpio *GPIOx, uint8 PinNum, uint8 State);
void GPIO_WriteGroup(gpio *GPIOx, uint16 Mask, uint16 Value);
uint8 GPIO_ReadPin(gpio *GPIOx, uint8 PinNum);

#endif
