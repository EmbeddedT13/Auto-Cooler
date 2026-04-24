# 1. Define the Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# 2. Define the Processor Architecture
MACH = cortex-m4
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -O0
LDFLAGS = -nostdlib -T stm32_ls.ld -Wl,-Map=project.map

# 3. Find all your source files based on the nested directory structure
SRC = startup.c \
      main.c \
      mcal/rcc/rcc.c \
      mcal/gpio/gpio.c \
      mcal/nvic/nvic.c \
      mcal/adc/adc.c \
      mcal/dma/dma.c \
      mcal/timer/timer.c \
      hal/led/led.c \
      hal/fan/fan.c \
      hal/lcd/lcd.c \
      hal/temp/temp.c \
      app/cooler_sm/cooler_sm.c \
      app/display/display.c

# 4. Convert .c filenames to .o filenames
OBJ = $(SRC:.c=.o)

# 5. The Output File Name
TARGET = stm32-cooler

# --- Build Rules ---
all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex $< $@
	@echo "Build Successful! Ready for Proteus."

$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean now digs into the nested folders
clean:
	rm -rf *.o mcal/*/*.o hal/*/*.o app/*/*.o *.elf *.hex *.map
