sinclude ../../rules.mk


CSRC = motor_control_with_encoder_and_systick.c
OBJ = $(CSRC:.c=.o)

motor_control_with_encoder_and_systick.elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

.PHONY: clean
clean:
	-rm -rf *.o *.bin *.elf *.map *.d

clean_all:
	-rm -rf *.o *.bin *.elf *.map *.d
	-$(MAKE) -C libopencm3 clean


%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

bin: motor_control_with_encoder_and_systick.bin

all: libopencm3 motor_control_with_encoder_and_systick.bin

