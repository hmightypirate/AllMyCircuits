#include "setup.h"
#include "utils.h"
#include "systick.h"
#include "lib_vl6180x.h"

struct choice_entry_st {
	char choice;
	void (*functionPtr)();
};

void single_range() {
	printf("\nSingle range test\n");

	// start single range measurement
	VL6180X_start_single_range();

	// poll the VL6180X till new sample ready
	VL6180X_poll_range();

	// read range result
	printf("%d millimeters\n", VL6180X_read_range());

	// clear the interrupt on VL6180X
	VL6180X_clear_interrupts();
}

void single_range_multiple_samples() {
	printf("\nMultiple samples of single range test\n");

	for (uint8_t i = 0; i < 10; i++) {
		// start single range measurement
		VL6180X_start_single_range();

		// poll the VL6180X till new sample ready
		VL6180X_poll_range();

		// read range result
		printf("%d millimeters\n", VL6180X_read_range());

		// clear the interrupt on VL6180X
		VL6180X_clear_interrupts();
	}
}

void continous_mode_sampling() {
	printf("\nContinuous mode sampling test\n");

	// start single range measurement
	VL6180X_start_continuous_range();

	while (1) {
		// poll the VL6180X till new sample ready
		VL6180X_poll_range();

		// read range result
		printf("%d millimeters\n", VL6180X_read_range());

		// clear the interrupt on VL6180X
		VL6180X_clear_interrupts();
	}
}

void example_2(int b) {
	printf("\nThis is Test 2 (%d)\n", b);
}

#define CHOICES_LEN 3
struct choice_entry_st choices[] = {
		{ .choice = '1', .functionPtr = single_range },
		{ .choice = '2', .functionPtr = single_range_multiple_samples },
		{ .choice = '3', .functionPtr = continous_mode_sampling }
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
