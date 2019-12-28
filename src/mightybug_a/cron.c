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


void delay_microseconds(uint32_t microseconds)
{
	// FIXME: Bug if now + microseconds * 72 is close to UINT32_MAX
	uint32_t now = dwt_read_cycle_counter();
	if (now + microseconds * 72 < microseconds * 72) {
		while (dwt_read_cycle_counter() > now);
	}
	
	while (dwt_read_cycle_counter() < now + microseconds * 72);

}