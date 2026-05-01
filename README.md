# Auto-Cooler: STM32F4 Closed-Loop Temperature Control

## Overview
The Auto-Cooler is a bare-metal embedded software project designed for the STM32F4 microcontroller. It implements a closed-loop control system that continuously monitors ambient temperature via an LM35 analog sensor and autonomously adjusts the speed of a DC cooling fan using Pulse-Width Modulation (PWM). 

System behavior is strictly governed by a **Mealy State Machine**, providing real-time feedback through a custom-written HD44780 LCD driver and a series of status LEDs. 

This project was built from scratch without the use of STM32 HAL libraries or external utilities, demonstrating direct register-level manipulation, hardware interrupts, and custom linker scripts.

---

## 🏗️ Software Architecture

The software is structured using a strict Layered Architecture to completely decouple application logic from hardware specifics:

*   **Application Layer (`app/`)**
    *   **Cooler State Machine:** A Mealy machine evaluating inputs and updating outputs.
    *   **Display Controller:** A UI layer utilizing a fixed-width string buffer and manual ASCII conversion (avoiding heavy `sprintf` calls) to prevent screen tearing/flickering.
*   **Hardware Abstraction Layer (`hal/`)**
    *   **LM35 Sensor:** Wraps the ADC driver to convert raw digital values into Celsius using fixed-point math.
    *   **DC Fan:** Wraps the Timer driver to translate 0-100% speed requests into PWM duty cycles.
    *   **HD44780 LCD:** Custom 4-bit parallel driver operating on precise SysTick microsecond delays.
    *   **Status LEDs:** Unified GPIO wrapper for system indicators.
*   **Microcontroller Abstraction Layer (`mcal/`)**
    *   **ADC:** Configured in Discontinuous (One-Shot) mode with an End-of-Conversion (EOC) interrupt to prevent CPU starvation. 
    *   **Timer:** Hardware PWM generation (TIM3) and deterministic SysTick delays.
    *   **RCC / GPIO / NVIC:** Core hardware initialization, clock gating, and interrupt routing.

---

## 🧠 State Machine Logic

The system runs on a highly controlled Producer-Consumer interrupt model. The ADC takes readings in the background and sets a flag. The main super-loop consumes this data and evaluates the following Mealy State Machine:

| State | Condition / Event | Fan Speed (PWM) | LED Indicator | LCD Status |
| :--- | :--- | :--- | :--- | :--- |
| **IDLE** | $T < 25.0^{\circ}C$ | 0% (OFF) | Green (SUCCESS) | Displays Temperature |
| **COOLING** | $25.0^{\circ}C \le T < 30.0^{\circ}C$ | 33% | Yellow (PROG_1) | Displays Temp & Speed |
| **COOLING** | $30.0^{\circ}C \le T < 35.0^{\circ}C$ | 66% | Yellow (PROG_1) | Displays Temp & Speed |
| **COOLING** | $35.0^{\circ}C \le T < 40.0^{\circ}C$ | 100% | Yellow (PROG_1) | Displays Temp & Speed |
| **OVERHEAT** | $T \ge 40.0^{\circ}C$ | 100% (Locked) | Red (ALARM) | "OVERHEAT!" Warning |
| **ERROR** | $T = 0.0^{\circ}C$ (Hardware Fault) | 0% (Safety Kill) | Blue (ERROR) | "OVERHEAT!" Warning |

*Note: The system features fault-tolerance. If the sensor is disconnected (reading 0), the system enters a safe ERROR state, kills the fan, and waits for sensor recovery.*

---

## 🔌 Hardware Pinout (Proteus Simulation)

| Component | STM32F401 Pin | Peripheral / Function |
| :--- | :--- | :--- |
| **LM35 Sensor (Vout)** | `PA0` | ADC Channel 0 |
| **L293D Motor Enable** | `PA6` | TIM3 Channel 1 (PWM) |
| **LCD RS / RW / EN** | `PB0`, `PB1`, `PB2` | GPIO Output |
| **LCD D4 - D7** | `PB4` - `PB7` | GPIO Output |
| **Green LED (IDLE)** | `PC0` | GPIO Output |
| **Yellow LED (COOLING)** | `PC1` | GPIO Output |
| **Red LED (OVERHEAT)** | `PC2` | GPIO Output |
| **Blue LED (ERROR)** | `PC3` | GPIO Output |

---

## 📸 Simulation Showcase

<img width="1564" height="885" alt="image" src="https://github.com/user-attachments/assets/a8b5ac1a-bce4-4d3f-afec-7792572556d7" />

---

## 🚀 Building and Running

This project uses the `arm-none-eabi-gcc` toolchain and a custom Makefile.

### Compilation
To compile the project into an `.elf` and `.hex` file:
1. Open your terminal in the root directory.
2. Run the clean command to remove old build artifacts:
   ```bash
   make clean
