#include "cron.h"

static volatile uint32_t millisecs_since_start = 0;

/*
 * @brief interrupt callback
 */
void sys_tick_handler(void)
{
	millisecs_since_start++;

	/* do encoder measurements

	   FIXME: check cron library

	 */
	update_encoder_ticks();
}

/*
 * @brief get the number of milliseconds since start
 */
uint32_t get_millisecs_since_start()
{
	return millisecs_since_start;
}
