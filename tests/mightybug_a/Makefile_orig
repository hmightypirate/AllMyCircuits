sinclude ../../rules.mk


CSRC = $(wildcard *.c) ../lib/Unity/src/unity.c ../lib/uart_putchar/uart_putchar.c
OBJ = $(CSRC:.c=.o)

BIN = test

LDLIBS := $(LDLIBS) -I../lib/Unity/src -I../lib/uart_putchar

TestDumbExample.o:
	arm-none-eabi-gcc -Wall -Wextra -g3 -O0 -MD -mcpu=cortex-m3 -mthumb -DSTM32F1 -std=c99 -DINTERNAL_LED=GPIO13 -DINTERNAL_LED_PORT=GPIOC -DRCC_INTERNAL_LED=RCC_GPIOC -I../../libopencm3//include -I../lib/Unity/src -I../lib/uart_putchar -c -o TestDumbExample.o TestDumbExample.c

$(BIN).elf: $(OBJ)
	echo $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d ../lib/Unity/src/*.o ../lib/Unity/src/*.d ../lib/uart_putchar/*.o ../lib/uart_putchar/*.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


$(BIN).bin: $(BIN).elf
	$(OBJCOPY) -O binary $< $@

bin: $(BIN).bin

all: libopencm3 $(BIN).bin
