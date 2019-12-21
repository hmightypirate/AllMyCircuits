sinclude ../../rules.mk

CSRC = $(wildcard *.c) ../lib/Unity/src/unity.c ../lib/uart_putchar/uart_putchar.c
LDLIBS := $(LDLIBS) -I../lib/Unity/src -I../lib/uart_putchar


TestDumbExample.bin:
	$(CC) $(TGT_CFLAGS) $(CFLAGS) -I../lib/Unity/src -I../lib/uart_putchar -c -o TestDumbExample.o TestDumbExample.c
	$(CC) $(TGT_CFLAGS) $(CFLAGS) -c -o DumbExample.o DumbExample.c
	$(CC) $(TGT_CFLAGS) $(CFLAGS) -c -o ../lib/Unity/src/unity.o ../lib/Unity/src/unity.c
	$(CC) $(TGT_CFLAGS) $(CFLAGS) -c -o ../lib/uart_putchar/uart_putchar.o ../lib/uart_putchar/uart_putchar.c
	$(CC) $(LDFLAGS) -o TestDumbExample.elf TestDumbExample.o DumbExample.o ../lib/Unity/src/unity.o ../lib/uart_putchar/uart_putchar.o -lopencm3_stm32f1 -lc -lnosys -I../lib/Unity/src -I../lib/uart_putchar
	$(OBJCOPY) -O binary TestDumbExample.elf TestDumbExample.bin


all: libopencm3 TestDumbExample.bin

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d ../lib/Unity/src/*.o \
		../lib/Unity/src/*.d ../lib/uart_putchar/*.o ../lib/uart_putchar/*.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean

