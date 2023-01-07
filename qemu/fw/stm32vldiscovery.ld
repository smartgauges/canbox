/* Linker script for ST STM32VLDISCOVERY (STM32F100RB, 128K flash, 8K RAM). */

/* Define memory regions. */
MEMORY
{
	rom (rx) : ORIGIN = 0x08000000, LENGTH = 128K
	ram (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

/* Include the common ld script. */
/*INCLUDE cortex-m-generic.ld*/
INCLUDE libopencm3_stm32f1.ld

