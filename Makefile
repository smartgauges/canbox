PREFIX=arm-none-eabi
CC=$(PREFIX)-gcc
OBJCOPY=$(PREFIX)-objcopy
AR=$(PREFIX)-ar

COMMON_INCLUDES=-I.
COMMON_CFLAGS=-Os -std=gnu99 -fno-common -ffunction-sections -fdata-sections -Wstrict-prototypes -Wundef -Wextra -Wshadow -Wredundant-decls
COMMON_LDFLAGS=--static -lc -lm -Wl,--cref -Wl,--gc-sections #-Wl,--print-gc-sections

PHONY:all

all: vw_nc03.bin volvo_od2.bin
#all: vw_nc03.bin
#all: volvo_od2.bin

########### vw_nc03 ##########
NUC131_BSP_DIR = vw_nc03/fw/nuc131bsp
NUC131_BSP_INCLUDES=-I$(NUC131_BSP_DIR)/Library/CMSIS/Include  -I$(NUC131_BSP_DIR)/Library/Device/Nuvoton/NUC131/Include -I$(NUC131_BSP_DIR)/Library/StdDriver/inc
NUC131_COMMON_FLAGS=-mcpu=cortex-m0 -mthumb -msoft-float -Wall
NUC131_INCLUDES=$(COMMON_INCLUDES) $(NUC131_BSP_INCLUDES)
NUC131_CFLAGS=$(NUC131_COMMON_FLAGS) $(NUC131_INCLUDES) $(COMMON_CFLAGS)
NUC131_LDFLAGS=$(NUC131_COMMON_FLAGS) $(COMMON_LDFLAGS)

%.vwo : %.c
	$(CC) $(NUC131_CFLAGS) -c $< -o $@

%.vwo : %.S
	$(CC) $(NUC131_CFLAGS) -c $< -o $@

NUC131_BSP_OBJS = $(NUC131_BSP_DIR)/Library/StdDriver/src/sys.vwo \
		$(NUC131_BSP_DIR)/Library/StdDriver/src/clk.vwo \
		$(NUC131_BSP_DIR)/Library/StdDriver/src/gpio.vwo \
		$(NUC131_BSP_DIR)/Library/StdDriver/src/uart.vwo \
		$(NUC131_BSP_DIR)/Library/StdDriver/src/can.vwo \
		$(NUC131_BSP_DIR)/Library/StdDriver/src/fmc.vwo \
		$(NUC131_BSP_DIR)/Library/Device/Nuvoton/NUC131/Source/system_NUC131.vwo \
		$(NUC131_BSP_DIR)/Library/Device/Nuvoton/NUC131/Source/GCC/startup_NUC131.o

VW_NC03_OBJS = vw_nc03/fw/hw_clock.vwo \
		vw_nc03/fw/hw_can.vwo \
		vw_nc03/fw/hw_gpio.vwo \
		vw_nc03/fw/hw_tick.vwo \
		vw_nc03/fw/hw_usart.vwo \
		vw_nc03/fw/hw_sleep.vwo \
		vw_nc03/fw/hw_conf.vwo

vw_nc03.bin: main.vwo canbox.vwo ring.vwo conf.vwo car.vwo tick.vwo hw.vwo sbrk.vwo $(VW_NC03_OBJS) $(NUC131_BSP_OBJS)
	$(CC) $^ -L. -T$(NUC131_BSP_DIR)/Library/Device/Nuvoton/NUC131/Source/GCC/gcc_arm.ld $(NUC131_LDFLAGS) $(NUC131_INCLUDES) -Wl,-Map=vw_nc03.map -o vw_nc03.elf
	$(OBJCOPY) -O binary vw_nc03.elf $@
########### vw_nc03 ##########

########### volvo_od2 ##########
LIBOPENCM3_DIR = volvo_od2/fw/libopencm3
LIBOPENCM3_INCLUDES = -I$(LIBOPENCM3_DIR)/include
F103_COMMON_FLAGS=-mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd -msoft-float -Wall
F103_INCLUDES=$(COMMON_INCLUDES) $(LIBOPENCM3_INCLUDES)
F103_CFLAGS=$(F103_COMMON_FLAGS) -DSTM32F1 $(F103_INCLUDES) $(COMMON_CFLAGS) --specs=nosys.specs 
F103_LDFLAGS=$(F103_COMMON_FLAGS) $(COMMON_LDFLAGS) --specs=nosys.specs -nostartfiles
%.vo : %.c
	$(CC) $(F103_CFLAGS) -c $< -o $@

LIBOPENCM3_OBJS = $(LIBOPENCM3_DIR)/lib/stm32/f1/gpio.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/f1/rcc.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/f1/pwr.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/f1/flash.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/gpio_common_all.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/usart_common_all.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/usart_common_f124.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/rcc_common_all.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/pwr_common_all.vo \
		$(LIBOPENCM3_DIR)/lib/cm3/vector.vo \
		$(LIBOPENCM3_DIR)/lib/cm3/systick.vo \
		$(LIBOPENCM3_DIR)/lib/cm3/nvic.vo \
		$(LIBOPENCM3_DIR)/lib/cm3/sync.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/flash_common_f01.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/can.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/iwdg_common_all.vo \
		$(LIBOPENCM3_DIR)/lib/stm32/common/exti_common_all.vo

VOLVO_OD2_OBJS = volvo_od2/fw/hw_clock.vo \
		volvo_od2/fw/hw_can.vo \
		volvo_od2/fw/hw_gpio.vo \
		volvo_od2/fw/hw_tick.vo \
		volvo_od2/fw/hw_usart.vo \
		volvo_od2/fw/hw_sleep.vo \
		volvo_od2/fw/hw_conf.vo

volvo_od2.bin: main.vo canbox.vo ring.vo conf.vo car.vo tick.vo hw.vo sbrk.vo $(VOLVO_OD2_OBJS) $(LIBOPENCM3_OBJS)
	$(CC) $^ -L. -Tvolvo_od2/fw/x32f103x8.ld -L$(LIBOPENCM3_DIR)/lib/stm32/f1 $(F103_LDFLAGS) $(F103_INCLUDES) -Wl,-Map=volvo_od2.map -o volvo_od2.elf
	$(OBJCOPY) -O binary volvo_od2.elf $@
########### volvo_od2 ##########

flash_vw_nc03: vw_nc03.bin
	/opt/openocd-nuvoton/bin/openocd -f vw_nc03/fw/nuc131.cfg -c "init; reset halt; numicro chip_erase;program vw_nc03.bin 0x0 verify; reset;exit"

flash_volvo_od2: volvo_od2.bin
	openocd -f volvo_od2/fw/stm32f103x8.cfg -c "init; reset halt" -c "mww 0x40022004 0x45670123" -c "mww 0x40022004 0xCDEF89AB" -c "mww 0x40022008 0x45670123" -c "mww 0x40022008 0xCDEF89AB" -c "mww 0x40022010 0x220" -c "mww 0x40022010 0x260" -c "sleep 100" -c "mww 0x40022010 0x230" -c "mwh 0x1ffff800 0x5AA5" -c "sleep 1000" -c "mww 0x40022010 0x2220" -c "sleep 100" -c "mdw 0x40022010" -c "mdw 0x4002201c" -c "mdw 0x1ffff800" -c targets -c "halt" -c "stm32f1x unlock 0;exit"
	openocd -f volvo_od2/fw/stm32f103x8.cfg -c "init; reset halt" -c "mww 0x40022004 0x45670123" -c "mww 0x40022004 0xCDEF89AB" -c "mww 0x40022008 0x45670123" -c "mww 0x40022008 0xCDEF89AB" -c targets -c "halt" -c "stm32f1x unlock 0; exit"
	openocd -f volvo_od2/fw/stm32f103x8.cfg -c "init; mdw 0x1ffff7e0; mdw 0x1ffff800; reset halt; stm32f1x unlock 0; sleep 1; program volvo_od2.bin 0x8000000; reset;exit"

test_nuc:
	/opt/openocd-nuvoton/bin/openocd -f vw_nc03/fw/nuc131.cfg -c "init; targets; halt; targets; mdw 0xe000ed00; mdw 0x50000000; reset; exit"

clean:
	rm -rf *.bin *.map $(NUC131_BSP_OBJS) $(VW_NC03_OBJS) *.vwo $(LIBOPENCM3_OBJS) $(VOLVO_OD2_OBJS) *.vo *.d *.elf

