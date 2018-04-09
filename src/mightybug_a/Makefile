sinclude ../../rules.mk


CSRC = $(wildcard *.c)
OBJ = $(CSRC:.c=.o)

agonizing_automobile.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: agonizing_automobile.bin

all: libopencm3 agonizing_automobile.bin

