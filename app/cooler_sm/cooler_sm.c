#include "cooler_sm.h"

#include "../display/display.h"
#include "../../hal/fan/fan.h"
#include "../../hal/led/led.h"
#include "../../hal/temp/temp.h"

static CoolerState_t CurrentState = COOLER_IDLE;

static uint8 COOLER_CalculateFanSpeed(uint16 temp_x10) {
    if (temp_x10 < THRESH_TEMP_LOW) return FAN_SPEED_OFF;       
    if (temp_x10 < THRESH_TEMP_MED) return FAN_SPEED_LOW;      
    if (temp_x10 < THRESH_TEMP_MAX) return FAN_SPEED_MED;      
    else return 100;                                    
}

void COOLER_Init(void){
    
    /* Initialize Subsystems */
    FAN_Init(); 
    TEMP_Init(TEMP_SENSOR_CHANNEL); 
    DISPLAY_Init();

    /* Initialize Status LEDs based on led.h Proteus mapping */
    LED_Init(LED_SUCCESS);  /* Green for IDLE */
    LED_Init(LED_PROG_1);   /* Yellow for COOLING */
    LED_Init(LED_ALARM);    /* Red for OVERHEAT */
    LED_Init(LED_ERROR);    /* Blue for SENSOR FAULT */

    /* Setup Initial Output State */
    FAN_SetSpeed(0);
    LED_On(LED_SUCCESS);
    LED_Off(LED_PROG_1);
    LED_Off(LED_ALARM);
    LED_Off(LED_ERROR);
}

void COOLER_Update(void) {
    
    /* Fetch the latest continuous reading */
    uint16 currentTemp = TEMP_GetCelsius_x10();
    uint8 fanSpeed = 0;

    /* --- HARDWARE FAULT OVERRIDE --- */
    /* If sensor drops to 0, force the state machine into the ERROR state */
    if (currentTemp == 0) {
        CurrentState = COOLER_ERROR;
    } 
    /* Recovery: If we were in an error, but the sensor comes back online */
    else if (CurrentState == COOLER_ERROR && currentTemp > 0) {
        CurrentState = COOLER_IDLE;
    }

    /* * MEALY MACHINE IMPLEMENTATION:
     * Outputs and Transitions are determined by (CurrentState + Input Event)
     */
    switch(CurrentState) {
        
        case COOLER_ERROR:
            /* Event: T == 0.0 °C (Disconnected or Broken Sensor) */
            fanSpeed = 0;
            FAN_SetSpeed(fanSpeed); /* Force Fan off for safety */
            
            /* Turn ON Blue Error LED, Turn OFF all others */
            LED_On(LED_ERROR);
            LED_Off(LED_SUCCESS);
            LED_Off(LED_PROG_1);
            LED_Off(LED_ALARM);
            
            /* Print 0 to Display, pass '1' for alarm text if supported */
            DISPLAY_Update(currentTemp, fanSpeed, 1); 
            break;

        case COOLER_IDLE:
            
            if (currentTemp >= THRESH_TEMP_LOW) { 
                /* Event: T >= 25.0 °C */
                CurrentState = COOLER_COOLING;
                fanSpeed = COOLER_CalculateFanSpeed(currentTemp);
                
                FAN_SetSpeed(fanSpeed);
                LED_Off(LED_SUCCESS);
                LED_On(LED_PROG_1);
                DISPLAY_Update(currentTemp, fanSpeed, 0);
                
            } else { 
                /* Event: 0.0 < T < 25.0 °C */
                CurrentState = COOLER_IDLE;
                fanSpeed = 0;
                
                FAN_SetSpeed(fanSpeed);
                LED_On(LED_SUCCESS);
                LED_Off(LED_ERROR); /* Ensure Blue LED is off after recovery */
                DISPLAY_Update(currentTemp, fanSpeed, 0);
            }
            break;
            
        case COOLER_COOLING:
            if (currentTemp < THRESH_TEMP_LOW) { 
                /* Event: T < 25.0 °C */
                CurrentState = COOLER_IDLE;
                fanSpeed = 0;
                
                FAN_SetSpeed(fanSpeed);
                LED_On(LED_SUCCESS);
                LED_Off(LED_PROG_1);
                DISPLAY_Update(currentTemp, fanSpeed, 0);
                
            } 
            else if (currentTemp >= THRESH_TEMP_ALARM) { 
                /* Event: Overheat Detected (T >= 40.0 °C) */
                CurrentState = COOLER_OVERHEAT;
                fanSpeed = 100;
                
                FAN_SetSpeed(fanSpeed);
                LED_Off(LED_PROG_1);
                LED_On(LED_ALARM);
                DISPLAY_Update(currentTemp, fanSpeed, 1);
                
            } 
            else { 
                /* Event: Normal Cooling Range (25.0 °C <= T < 40.0 °C) */
                CurrentState = COOLER_COOLING;
                fanSpeed = COOLER_CalculateFanSpeed(currentTemp);
                
                FAN_SetSpeed(fanSpeed);
                DISPLAY_Update(currentTemp, fanSpeed, 0);
            }
            break;
            
        case COOLER_OVERHEAT:
            if (currentTemp < THRESH_TEMP_ALARM) { 
                /* Event: T < 40.0 °C */
                CurrentState = COOLER_COOLING;
                fanSpeed = COOLER_CalculateFanSpeed(currentTemp);
                
                FAN_SetSpeed(fanSpeed);
                LED_Off(LED_ALARM);
                LED_On(LED_PROG_1);
                DISPLAY_Update(currentTemp, fanSpeed, 0);
                
            } 
            else { 
                /* Event: T >= 40.0 °C */
                CurrentState = COOLER_OVERHEAT;
                fanSpeed = 100;
                
                FAN_SetSpeed(fanSpeed);
                DISPLAY_Update(currentTemp, fanSpeed, 1);
            }
            break;
    }
}