PREFIX=arm-none-eabi
CC=$(PREFIX)-gcc
OBJCOPY=$(PREFIX)-objcopy
AR=$(PREFIX)-ar

COMMON_INCLUDES=-I.
COMMON_CFLAGS=-Os -std=gnu99 -fno-common -ffunction-sections -fdata-sections -Wstrict-prototypes -Wundef -Wextra -Wshadow -Wredundant-decls #-Waddress-of-packed-member
COMMON_LDFLAGS=--static -lc -lm -Wl,--cref -Wl,--gc-sections #-Wl,--print-gc-sections

PHONY:all

all: vw_nc03.bin volvo_od2.bin qemu.bin

include Makefile_stm32f1
include Makefile_volvo_od2
include Makefile_vw_nc03
include Makefile_qemu

clean:
	rm -rf *.bin *.map $(NUC131_BSP_OBJS) $(VW_NC03_OBJS) *.vwo $(LIBOPENCM3_OBJS) $(VOLVO_OD2_OBJS) *.vo $(QEMU_OBJS) *.qemu *.d *.elf

