sinclude ../../rules.mk


CSRC = $(wildcard *.c)
OBJ = $(CSRC:.c=.o)

sensor_test.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: sensor_test.bin

all: libopencm3 sensor_test.bin
