sinclude ../../rules.mk


CSRC = $(wildcard *.c)
OBJ = $(CSRC:.c=.o)

BIN = buzzer_music

$(BIN).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


$(BIN).bin: $(BIN).elf
	$(OBJCOPY) -O binary $< $@

bin: $(BIN).bin

all: libopencm3 $(BIN).bin
