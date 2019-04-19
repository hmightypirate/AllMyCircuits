#include "lib_sd_raw.h"
#include "systick.h"

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "lib_spi_master.h"


static uint8_t response[5];
static uint8_t argument[4];


uint8_t lib_sd_raw_setup() {

	setup_card_select();

	sleep_ms(250);

	/* De-select the Card: Chip Select high */
	gpio_set(GPIOB, GPIO12);

	lib_spi_master_setup();

	/* Select the Card: Chip Select low */
	gpio_clear(GPIOB, GPIO12);

	lib_spi_master_drain_spi();

	return 1;
}

void setup_card_select(){
	gpio_set_mode(
			GPIOB,
			GPIO_MODE_OUTPUT_50_MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			GPIO12);
}


uint8_t sd_card_present(){return 1;}


uint8_t sd_initialize(sd_context_t *sdc) {
	uint8_t i, j;
	/* SPI SD initialization sequence:
	 * CMD0
	 * CMD55
	 * ACMD41
	 * CMD58
	 */

//	sdc->busyflag = 0;
//	for (i = 0; i < 4; i++)
//		argument[i] = 0;
//	/* Delay for at least 74 clock cycles. This means to actually
//	 * *clock* out at least 74 clock cycles with no data present on
//	 * the clock. In SPI mode, send at least 10 idle bytes (0xFF). */
//	spi_cs_assert();
//	sd_delay(100);
//	spi_cs_deassert();
//	sd_delay(2);
//	/* Put the card in the idle state */
//	if (sd_send_command(sdc, CMD0, CMD0_R, response, argument) == 0)
//		return 0;
//	/* Now wait until the card goes idle. Retry at most SD_IDLE_WAIT_MAX
//	 times */
//	j = 0;
//	do {
//		j++;
//		/* Flag the next command as an application-specific command */
//		if (sd_send_command(sdc, CMD55, CMD55_R, response, argument) == 1) {
//			/* Tell the card to send its OCR */
//			sd_send_command(sdc, ACMD41, ACMD41_R, response, argument);
//		} else {
//			/* No response, bail early */
//			j = SD_IDLE_WAIT_MAX;
//		}
//	} while ((response[0] & MSK_IDLE) == MSK_IDLE && j < SD_IDLE_WAIT_MAX);
//	/* As long as we didn’t hit the timeout, assume we’re OK. */
//	if (j >= SD_IDLE_WAIT_MAX)
//		return 0;
//	if (sd_send_command(sdc, CMD58, CMD58_R, response, argument) == 0)
//		return 0;
//	/* At a very minimum, we must allow 3.3V. */
//	if ((response[2] & MSK_OCR_33) != MSK_OCR_33)
//		return 0;
//	/* Set the block length */
//	if (sd_set_blocklen(sdc, SD_BLOCKSIZE) != 1)
//		return 0;
//	/* If we got this far, initialization was OK.*/
	return 1;
}

uint8_t sd_read_block (sd_context_t *sdc, uint32_t blockaddr, uint8_t *data){return 1;}
uint8_t sd_write_block (sd_context_t *sdc, uint32_t blockaddr, uint8_t *data){return 1;}
void sd_wait_notbusy (sd_context_t *sdc){}
/* Internal functions, used for SD card communications. */
void sd_packarg(uint32_t *argument, uint32_t value){}
uint8_t sd_set_blocklen (sd_context_t *sdc, uint32_t length){return 1;}
uint8_t sd_send_command(sd_context_t *sdc,
		uint8_t cmd, uint8_t response_type,
		uint8_t *response, uint8_t *argument){return 1;}
void sd_delay(uint32_t number){return 1;}
