/* stm32_ls.ld */
ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
  SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
  .text :
  {
    *(.isr_vector) /* Vector table MUST be first */
    *(.text)       /* Program code */
    *(.rodata)     /* Read-only data (like your 7-seg lookup table!) */
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    _sdata = .;
    *(.data)       /* Initialized variables */
    . = ALIGN(4);
    _edata = .;
  } > SRAM AT > FLASH

  .bss :
  {
    _sbss = .;
    *(.bss)        /* Uninitialized variables */
    . = ALIGN(4);
    _ebss = .;
  } > SRAM
}