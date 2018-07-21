sinclude ../../rules.mk


CSRC = timer1_pwms.c
OBJ = $(CSRC:.c=.o)

timer1_pwms.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: timer1_pwms.bin

all: libopencm3 timer1_pwms.bin

