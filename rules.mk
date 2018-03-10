CROSS_COMPILE = arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
CPUFLAGS = -mcpu=cortex-m3 -mthumb
LIBPATHS := ./libopencm3 ../../libopencm3 

OPENCM3_DIR := $(wildcard $(LIBPATHS:=/locm3.sublime-project))
OPENCM3_DIR := $(firstword $(dir $(OPENCM3_DIR)))



ifeq ($(strip $(OPENCM3_DIR)),)
$(warning Cannot find libopencm3 library in the standard search paths.)
$(error Please specify it through OPENCM3_DIR variable!)
endif


CFLAGS = -Wall -Wextra -g3 -O0 -MD $(CPUFLAGS) -DSTM32F1 -I${OPENCM3_DIR}/include -std=c99

LDLIBS = -lopencm3_stm32f1 -lc -lnosys


LIBPATHS := . ../../
BLUE_DIR := $(wildcard $(LIBPATHS:=/bluepill.ld))
BLUE_DIR := $(firstword $(dir $(BLUE_DIR)))
ifeq ($(strip $(BLUE_DIR)),)
$(warning Cannot find bluepill.ld in the standard search paths.)
$(error Please specify it through BLUE_DIR variable!)
endif


LDFLAGS = $(CPUFLAGS) -nostartfiles -L${OPENCM3_DIR}/lib -Wl,-T,${BLUE_DIR}/bluepill.ld

.PHONY: libopencm3
libopencm3:
	if [ ! -f ${OPENCM3_DIR}/Makefile ]; then \
		git submodule init; \
		git submodule update; \
	fi
	$(MAKE) -C ${OPENCM3_DIR} lib/stm32/f1


