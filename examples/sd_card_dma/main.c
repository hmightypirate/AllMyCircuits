#include "lib_sd_raw.h"
#include "setup.h"
#include "utils.h"
#include "systick.h"

struct choice_entry_st {
	char choice;
	void (*functionPtr)();
};

void test_1() {
	printf("\ntest_1\n");

}

void test_2() {
	printf("\ntest_2\n");

}

void test_3() {
	printf("\ntest_3\n");

}

#define CHOICES_LEN 3
struct choice_entry_st choices[] = {
		{ .choice = '1', .functionPtr = test_1 },
		{ .choice = '2', .functionPtr = test_2 },
		{ .choice = '3', .functionPtr = test_3 }
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
