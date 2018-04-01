#include <stdlib.h>
#include <libopencm3/cm3/systick.h>
#include "cron.h"

// Array of functions to call
function_pointer functions[MAXFUNC];

// Variable to store system time. It overflows every 4.294.967.295ms ~= 49,71 days
uint32_t systime[MAXFUNC];

// This callback is called once every 1ms
void sys_tick_handler(void) {
    for(int i=0;i<MAXFUNC;i++) {
        if(functions[i] != NULL) {
            systime[i]++;
            functions[i](systime[i]);
        }
    }
}

// Setup systick and start calling the handler
void systick_setup(){
   /* 72MHz / 8 => 9000000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();
}

// Initialise the cron service
void cron_setup() {
    for(int i=0;i<MAXFUNC;i++) {
        functions[i] = NULL;
        systime[i] = 0;
    }
    systick_setup();
}

// Add a new function to be called every tick
// @returns A handler of the function to be able to delete it later or -1 if there was an error.
// @params A function pointer to be called every tick. It should be as short as possible.
int cron_add(function_pointer newfunc) {
    for(int i=0;i<MAXFUNC;i++) {
        if(functions[i] == NULL) {
            functions[i] = newfunc;
            return i;
        }
    }
    return -1;
}

// Delete a function from the list. It will not be called again every tick.
// @params The handler returned by cron_add when the function was added
void cron_delete(int i) {
    functions[i] = NULL;
    systime[i] = 0;
}
