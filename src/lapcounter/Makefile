sinclude ../../rules.mk

NAME = lapcounter

# BUILD BINS
CSRC = $(NAME).c
OBJ = $(CSRC:.c=.o)
BIN = $(NAME).bin

# USE ALLMYLIBS
LDFLAGS += -L../../allmylibs/lib
CFLAGS += -I../../allmylibs/include
LDLIBS := $(LDLIBS)

################################
# DO NOT MODIFY BELOW THIS LINE
################################
%.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

mrproper:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: $(BIN)

all: libopencm3 $(BIN)
