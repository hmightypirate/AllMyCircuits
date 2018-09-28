#include "libwhatever.h"
#include "setup.h"
#include "utils.h"
#include "systick.h"


struct choice_entry_st{
	char choice;
	void (*functionPtr)(int);
};


void example_1(int a){
	printf("\nThis is Test 1 (%d)\n", a);
}


void example_2(int b){
	printf("\nThis is Test 2 (%d)\n", b);
}

#define CHOICES_LEN 2
struct choice_entry_st choices[] = {
		{.choice = '1', .functionPtr = example_1},
		{.choice = '2', .functionPtr = example_2}
};

int main(void) {
	setup();

	while(1){

		char user_choice = prompt();

		for (int i = 0; i < CHOICES_LEN; i++){
			if (choices[i].choice == user_choice){
				uint32_t c = get_millisecs_since_start();
				choices[i].functionPtr(123);
				c = get_millisecs_since_start() - c;
				printf("\nTest has lasted %lu ms\n", c);
			}
		}
	}
}
