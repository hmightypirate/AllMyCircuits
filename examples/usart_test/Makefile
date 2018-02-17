sinclude ../../rules.mk


CSRC = usart_test.c
OBJ = $(CSRC:.c=.o)

usart_test.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: usart_test.bin

all: libopencm3 usart_test.bin

