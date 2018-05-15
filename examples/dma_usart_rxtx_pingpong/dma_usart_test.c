#include "fill_and_send_example.h"
#include "ping_pong_example.h"
#include "dma_example.h"
#include "setup.h"
#include "utils.h"
#include "systick.h"

#define EXAMPLE_LOOPS 10

struct choice_entry_st{
	char choice;
	void (*functionPtr)(int);
};

#define CHOICES_LEN 3
struct choice_entry_st choices[] = {
		{.choice = '1', .functionPtr = fill_and_send_example},
		{.choice = '2', .functionPtr = ping_pong_example},
		{.choice = '3', .functionPtr = dma_example}
};

int main(void) {
	setup();

	while(1){

		char user_choice = prompt();

		for (int i = 0; i < CHOICES_LEN; i++){
			if (choices[i].choice == user_choice){
				uint32_t c = get_millisecs_since_start();
				choices[i].functionPtr(EXAMPLE_LOOPS);
				c = get_millisecs_since_start() - c;
				printf("\nTest has used %lu ms\n", c);
			}
		}
	}
}
