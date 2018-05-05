sinclude ../../rules.mk


CSRC = $(wildcard *.c)
OBJ = $(CSRC:.c=.o)

keypad.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: keypad.bin

all: libopencm3 keypad.bin
