sinclude ../../rules.mk

VERSION_NAME=disable_drive
BIN_PATH=./bin

CSRC = $(wildcard *.c)
OBJ_NO_DIR = $(CSRC:%.c=%.o)
OBJ = $(patsubst %.c,$(BIN_PATH)/%.o,$(CSRC))

bin/%.o: %.c
	@printf "  CC      $(<F)\n"
	mkdir -p $(BIN_PATH)
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) -o $@ -c $<

$(VERSION_NAME).elf: $(OBJ)
	mkdir -p $(BIN_PATH)
	$(CC) $(LDFLAGS) -o $(BIN_PATH)/$@ $(OBJ) $(LDLIBS) $(CFLAGS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d ./bin

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $(BIN_PATH)/$< $(BIN_PATH)/$@
	$(info ‘text’ is what ends up typically in FLASH and has code and constant data.)
	$(info ‘data’ is used for initialized data.)
	$(info ‘bss’ contains all the uninitalized data.)
	arm-none-eabi-size --format=Berkeley -d $(BIN_PATH)/$(VERSION_NAME).elf

bin: $(VERSION_NAME).bin

all: libopencm3 $(VERSION_NAME).bin

