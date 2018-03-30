sinclude ../../rules.mk

# BUILD BINS
CSRC = test_cron.c cron.c buzzer.c
OBJ = $(CSRC:.c=.o)

%.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: test_cron.bin

all: libopencm3 test_cron.bin
