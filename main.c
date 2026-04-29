#include "app/cooler_sm/cooler_sm.h"
#include "mcal/timer/timer.h" 

int main(void) {
    /* 1. Initialize the State Machine and all underlying MCAL/HAL drivers */
    COOLER_Init();

    /* 2. The Super Loop */
    while (1) {
        /* Run the Mealy State Machine logic */
        COOLER_Update();
        
        /* A 250ms delay keeps the LCD from flickering and allows 
           us to easily read the live temperature updates. */
        Timer_Delay_ms(250); 
    }

    return 0;
}
