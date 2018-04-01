// Maximum number of functions allowed in the cron list
#define MAXFUNC 5

// Function type to be used with cron service.
// It receives one parameter with the system ticks since the service was started.
typedef void (*function_pointer)(uint32_t);

// Setup systick and start calling the handler
void systick_setup();

// Initialise the cron service
void cron_setup();

// Add a new function to be called every tick
// @returns A handler of the function to be able to delete it later or -1 if there was an error.
// @params A function pointer to be called every tick. It should be as short as possible.
int cron_add(function_pointer newfunc);

// Delete a function from the list. It will not be called again every tick.
// @params The handler returned by cron_add when the function was added
void cron_delete(int i);
