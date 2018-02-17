CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
CPUFLAGS = -mcpu=cortex-m3 -mthumb
CFLAGS = -Wall -Wextra -g3 -O0 -MD $(CPUFLAGS) -DSTM32F1 -Ilibopencm3/include -std=c99
LDFLAGS = $(CPUFLAGS) -nostartfiles -Llibopencm3/lib -Wl,-T,bluepill.ld
LDLIBS = -lopencm3_stm32f1 -lc -lnosys 

CSRC = bluepill_test.c
OBJ = $(CSRC:.c=.o)

bluepill_test.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: libopencm3
libopencm3:
	if [ ! -f libopencm3/Makefile ]; then \
		git submodule init; \
		git submodule update; \
	fi
	$(MAKE) -C libopencm3 lib/stm32/f1

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: bluepill_test.bin

all: libopencm3 bluepill_test.bin
