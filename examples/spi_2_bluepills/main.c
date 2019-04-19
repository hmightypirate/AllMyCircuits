#include "lib_sd_raw.h"
#include "setup.h"
#include "utils.h"
#include "systick.h"

#include "lib_spi_slave.h"
#include "lib_spi_master.h"

struct choice_entry_st {
	char choice;
	void (*functionPtr)();
};

void slave_loop() {
	printf("\slave_loop\n");

}

void master_loop() {
	printf("\master_loop\n");
	lib_spi_master_setup();

	uint8_t i = 0;
	for (;i < 100; i++){
		lib_spi_master_write_byte(i);
		printf("%d\n", i);
	}
}

void master_slave_loop() {
	printf("\master_slave_loop\n");

	lib_spi_master_setup();
	lib_spi_slave_setup();

	uint8_t i = 0;
	for (;i < 100; i++){
		lib_spi_master_write_byte(i);
		uint8_t a = lib_spi_slave_read_byte();
		printf("%d -> %d\n", i, a);
	}
}

#define CHOICES_LEN 3
struct choice_entry_st choices[] = {
		{ .choice = '1', .functionPtr = slave_loop },
		{ .choice = '2', .functionPtr = master_loop },
		{ .choice = '3', .functionPtr = master_slave_loop }
		};

int main(void) {
	uint8_t res = setup();
	if (res != SETUP_OK) {
		printf("Setup error code %d\n", res);
		while (1){};
	}

	while (1) {

		char user_choice = prompt();

		for (int i = 0; i < CHOICES_LEN; i++) {
			if (choices[i].choice == user_choice) {
				uint32_t c = get_millisecs_since_start();
				choices[i].functionPtr();
				c = get_millisecs_since_start() - c;
				printf("\nTest has lasted %lu ms\n", c);
			}
		}
	}
}
