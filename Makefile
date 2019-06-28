VERSION_NAME=electronic_experience
BIN_PATH=./bin

CSRC = $(wildcard *.c)
OBJ_NO_DIR = $(CSRC:%.c=%.o)
OBJ = $(patsubst %.c,$(BIN_PATH)/%.o,$(CSRC))

DOCKER_IMAGE_NAME = mighty-arm-development
PORT = /dev/ttyUSB0
EXAMPLE = bluepill_test

build_image:
	docker build -t $(DOCKER_IMAGE_NAME) .

rm_image:
	docker rmi -f $(DOCKER_IMAGE_NAME)

console:
	docker run -v $(PWD):/home/src/ \
		-ti \
		--rm \
		--device=/dev/ttyUSB0 \
		$(DOCKER_IMAGE_NAME)

buga_fw: ## Build line follower firmware at src/mightybug_a
	docker run -v $(PWD):/home/src/ \
		-ti \
		--rm \
		$(DOCKER_IMAGE_NAME) \
		make -C src/mightybug_a/ clean all

buga_fw_flash: ## Flash firmware to the board at PORT (default: /dev/ttyUSB0). Example: make buga_fw_flash PORT=/dev/ttyUSB3
	docker run -v $(PWD):/home/src/ \
		-ti \
		--rm \
		--device=$(PORT) \
		$(DOCKER_IMAGE_NAME) \
		stm32loader -p $(PORT) -e -w -V -g 0x08000000 -v src/mightybug_a/bin/electronic_experience.bin

EXAMPLES := $(sort $(wildcard examples/*))
FLASH := $(addsuffix _flash, $(EXAMPLES))

$(EXAMPLES): BINARY = $(subst examples/,,$@)
$(EXAMPLES):
	docker run -v $(PWD):/home/src/ \
		-ti \
		--rm \
		$(DOCKER_IMAGE_NAME) \
		make -C $@ clean all BIN=$(BINARY)

$(FLASH): FOLDER = $(subst _flash,,$@)
$(FLASH): BINARY = $(subst examples/,,$(FOLDER))
$(FLASH):
	docker run -v $(PWD):/home/src/ \
		-ti \
		--rm \
		--device=$(PORT) \
		$(DOCKER_IMAGE_NAME) \
		stm32loader -p $(PORT) -e -w -V -g 0x08000000 -v $(FOLDER)/$(BINARY).bin

.PHONY: $(EXAMPLES) $(FLASH)

help:
	@printf '\033[33mBase targets:\033[0m\n'
	@printf '\033[36m    %-25s\033[0m %s\n' 'build_image' 'Generate Docker development image. Includes ARM GCC toolchain and stm32loader.'
	@printf '\033[36m    %-25s\033[0m %s\n' 'rm_image' 'Delete Docker development image.'
	@printf '\033[36m    %-25s\033[0m %s\n' 'console' 'Run Docker container with an interactive bash terminal'
	@printf '\033[36m    %-25s\033[0m %s\n' 'buga_fw' 'Build line follower firwmare at src/mightybug_a.'
	@printf '\033[36m    %-25s\033[0m %s\n' 'buga_fw_flash' 'Load firmware to the board connected to PORT (default: /dev/ttyUSB0). Example: make buga_fw_flash PORT=/dev/ttyUSB3'
	@printf '\n'
	@printf '\033[33m%-29s\033[0m Targets to build each example. Add \033[36m"_flash"\033[0m suffix to load the corresponding example to the board\n' 'Examples:'
	@for v in $(EXAMPLES) ; do \
		printf '\033[36m    %s\033[0m\n' $$v; \
    done

.DEFAULT_GOAL := help
