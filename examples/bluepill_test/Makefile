sinclude ../../rules.mk


CSRC = bluepill_test.c
OBJ = $(CSRC:.c=.o)

bluepill_test.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

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

