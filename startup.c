#include <stdint.h>

/* Memory boundaries imported from the linker script */
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _sidata;

/* Forward declarations */
int main(void);
void EXTI9_5_IRQHandler(void);
void EXTI0_IRQHandler(void);

/* Calculate the top of the SRAM (0x20000000 + 96KB) */
#define SRAM_START 0x20000000U
#define SRAM_SIZE  (96U * 1024U)
#define SRAM_END   (SRAM_START + SRAM_SIZE)
#define STACK_PTR  SRAM_END

/* Default infinite loop for unused interrupts */
void Default_Handler(void) {
    while(1);
}

/* The Boot Function */
void Reset_Handler(void) {
    /* 1. Copy .data section from Flash to RAM */
    uint32_t *pSrc = &_sidata;
    uint32_t *pDest = &_sdata;
    while(pDest < &_edata) {
        *pDest++ = *pSrc++;
    }

    /* 2. Initialize .bss section to zero in RAM */
    pDest = &_sbss;
    while(pDest < &_ebss) {
        *pDest++ = 0;
    }

    /* 3. Jump to your main application! */
    main();
    while(1);
}

/* The Vector Table (Exactly mapping STM32F401 IRQs) */
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))STACK_PTR, /* 0: Top of Stack */
    Reset_Handler,             /* 1: Reset Handler */
    Default_Handler,           /* 2: NMI */
    Default_Handler,           /* 3: Hard Fault */
    Default_Handler,           /* 4: Memory Management */
    Default_Handler,           /* 5: Bus Fault */
    Default_Handler,           /* 6: Usage Fault */
    0, 0, 0, 0,                /* 7-10: Reserved */
    Default_Handler,           /* 11: SVCall */
    Default_Handler,           /* 12: Debug Monitor */
    0,                         /* 13: Reserved */
    Default_Handler,           /* 14: PendSV */
    Default_Handler,           /* 15: SysTick */
    
    /* External Interrupts start at index 16 */
    Default_Handler, /* IRQ 0  (Index 16) */
    Default_Handler, /* IRQ 1 */
    Default_Handler, /* IRQ 2 */
    Default_Handler, /* IRQ 3 */
    Default_Handler, /* IRQ 4 */
    Default_Handler, /* IRQ 5 */
    EXTI0_IRQHandler, /* IRQ 6  - EXTI0 */
    Default_Handler, /* IRQ 7  - EXTI1 */
    Default_Handler, /* IRQ 8  - EXTI2 */
    Default_Handler, /* IRQ 9  - EXTI3 */
    Default_Handler, /* IRQ 10 - EXTI4 */
    Default_Handler, /* IRQ 11 */
    Default_Handler, /* IRQ 12 */
    Default_Handler, /* IRQ 13 */
    Default_Handler, /* IRQ 14 */
    Default_Handler, /* IRQ 15 */
    Default_Handler, /* IRQ 16 */
    Default_Handler, /* IRQ 17 */
    Default_Handler, /* IRQ 18 */
    Default_Handler, /* IRQ 19 */
    Default_Handler, /* IRQ 20 */
    Default_Handler, /* IRQ 21 */
    Default_Handler, /* IRQ 22 */
    EXTI9_5_IRQHandler /* IRQ 23 (Index 39): Your Doorbell & Reset! */
};