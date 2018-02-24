sinclude ../../rules.mk


CSRC = timer4_pwms.c
OBJ = $(CSRC:.c=.o)

timer4_pwms.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: timer4_pwms.bin

all: libopencm3 timer4_pwms.bin

