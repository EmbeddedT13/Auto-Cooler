#include "temp.h"
#include "../../mcal/adc/adc.h"

/* Temp_Buffer and Temp_DataReady are defined in adc.c */

void TEMP_Init(uint8 Channel) {
    /* Single-conversion interrupt-driven mode */
    ADC_Init(Channel, ADC_RESOLUTION_12B, ADC_DISCONTINUOUS);
    ADC_EnableInterrupt();
    ADC_Start(); /* Power on + trigger first conversion */
}

uint16 TEMP_GetCelsius_x10(void) {

    uint16 raw_digital;

    /* --- Atomic read of shared ISR buffer ---
     * Disable ADC IRQ (bit 18) while copying, so the ISR cannot
     * corrupt raw_digital mid-read. Re-enable immediately after. */
    #define NVIC_ICER0 (*((volatile uint32*)0xE000E180))
    #define NVIC_ISER0 (*((volatile uint32*)0xE000E100))

    NVIC_ICER0 = (1u << 18);          /* Disable ADC IRQ */
    raw_digital = Temp_Buffer;        /* Safe copy */
    Temp_DataReady = 0;               /* Mark as consumed */
    NVIC_ISER0  = (1u << 18);         /* Re-enable ADC IRQ */

    /* Retrigger the next conversion now that we've consumed the last result */
    ADC_TriggerNext();

    /* LM35: 10mV per °C. Vref=3300mV, 12-bit resolution=4096 steps.
     * milliVolts = raw * 3300 / 4096
     * temp_x10   = milliVolts  (because 10mV/°C → mV value = temp × 10) */
    uint32 milliVolts = ((uint32)raw_digital * TEMP_VOLTAGE) / TEMP_RESOLUTION;

    return (uint16)milliVolts;
}