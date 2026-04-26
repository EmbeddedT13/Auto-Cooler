#ifndef NVIC_H
#define NVIC_H

#include "../../core/BIT_MATH.h"
#include "../../core/STD_TYPES.h"

/* NVIC Base Address */
#define NVIC_BASE_ADDRESS 0xE000E100

/* Core Interrupt Set-Enable Registers */
#define NVIC_ISER0     *((volatile uint32*)(NVIC_BASE_ADDRESS + 0x000)) /* Enable external interrupts from 0 to 31  */
#define NVIC_ISER1     *((volatile uint32*)(NVIC_BASE_ADDRESS + 0x004)) /* Enable external interrupts from 32 to 63 */
#define NVIC_ICER0     *((volatile uint32*)(NVIC_BASE_ADDRESS + 0x080)) /* Disable external interrupts from 0 to 31 */
#define NVIC_ICER1     *((volatile uint32*)(NVIC_BASE_ADDRESS + 0x084)) /* Disable external interrupts from 32 to 63 */


#define NVIC_IPR        ((volatile uint8*) (NVIC_BASE_ADDRESS + 0x300)) /* Interrupt Priority Registers (Byte-accessible) */

/* Priority Levels */
#define NVIC_PRIORITY_URGENT  0
#define NVIC_PRIORITY_NORMAL  1

/* EXTI IRQ Number Mappings */
#define NVIC_IRQ_EXTI0          6
#define NVIC_IRQ_EXTI1          7
#define NVIC_IRQ_EXTI2          8
#define NVIC_IRQ_EXTI3          9
#define NVIC_IRQ_EXTI4          10
#define NVIC_ADC_IRQ            18
#define NVIC_IRQ_EXTI9_5        23
#define NVIC_TIM3_IRQ           29
#define NVIC_IRQ_EXTI15_10      40
#define NVIC_DMA2_STREAM0_IRQ   56 


void NVIC_EnableInterrupt(uint8 IRQ_Num);
void NVIC_DisableInterrupt(uint8 IRQ_Num);
void NVIC_SetPriority(uint8 IRQ_Num, uint8 Priority);

#endif
