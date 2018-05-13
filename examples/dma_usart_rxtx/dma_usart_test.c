#include "fill_and_send_example.h"
#include "ping_pong_example.h"
#include "setup.h"
#include "utils.h"
#include "systick.h"

#define EXAMPLE_LOOPS 10

int main(void) {
	setup();

	uint32_t c = get_millisecs_since_start();
	for (int i = 0 ; i < 800000; i++){
		__asm__("nop");
	}
	c = get_millisecs_since_start() - c;
	printf("\nTest has used %lu ms\n", c);

	while(1){

		char choice = prompt();

		if (choice == '1'){
			uint32_t c = get_millisecs_since_start();
			fill_and_send_example(EXAMPLE_LOOPS);
			c = get_millisecs_since_start() - c;
			printf("\nTest has used %lu ms\n", c);
		}

		if (choice == '2'){
			uint32_t c = get_millisecs_since_start();
			ping_pong_example(EXAMPLE_LOOPS);
			c = get_millisecs_since_start() - c;
			printf("\nTest has used %lu ms\n", c);
		}
	}
}
