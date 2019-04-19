
#ifndef LIB_SD_RAW_H_
#define LIB_SD_RAW_H_

#include <stdint.h>


// returns -1 if could not be initialized
uint8_t lib_sd_raw_setup();


/* response lengths */
#define R1 1
#define R1B 2
#define R2 3
#define R3 4

/******************************** Basic command set **************************/
/* Reset cards to idle state */
#define CMD0 0
#define CMD0_R R1
/* Read the OCR (MMC mode, do not use for SD cards) */
#define CMD1 1
#define CMD1_R R1
/* Card sends the CSD */
#define CMD9 9
#define CMD9_R R1
/* Card sends CID */
#define CMD10 10
#define CMD10_R R1
/* Stop a multiple block (stream) read/write operation */
#define CMD12 12
#define CMD12_R R1B
/* Get the addressed card’s status register */
#define CMD13 13
#define CMD13_R R2

/***************************** Block read commands **************************/
/* Set the block length */
#define CMD16 16
#define CMD16_R R1
/* Read a single block */
#define CMD17 17
#define CMD17_R R1
/* Read multiple blocks until a CMD12 */
#define CMD18 18
#define CMD18_R R1

/***************************** Block write commands *************************/
/* Write a block of the size selected with CMD16 */
#define CMD24 24
#define CMD24_R R1
/* Multiple block write until a CMD12 */
#define CMD25 25
#define CMD25_R R1
/* Program the programmable bits of the CSD */
#define CMD27 27
#define CMD27_R R1

/***************************** Write protection *****************************/
/* Set the write protection bit of the addressed group */
#define CMD28 28
#define CMD28_R R1B
/* Clear the write protection bit of the addressed group */
#define CMD29 29
#define CMD29_R R1B
/* Ask the card for the status of the write protection bits */
#define CMD30 30
#define CMD30_R R1

/***************************** Erase commands *******************************/
/* Set the address of the first write block to be erased */
#define CMD32 32
#define CMD32_R R1
/* Set the address of the last write block to be erased */
#define CMD33 33
#define CMD33_R R1
/* Erase the selected write blocks */
#define CMD38 38
#define CMD38_R R1B

/***************************** Lock Card commands ***************************/
/* Commands from 42 to 54, not defined here */
/***************************** Application-specific commands ****************/
/* Flag that the next command is application-specific */
#define CMD55 55
#define CMD55_R R1
/* General purpose I/O for application-specific commands */
#define CMD56 56
#define CMD56_R R1
/* Read the OCR (SPI mode only) */
#define CMD58 58
#define CMD58_R R3
/* Turn CRC on or off */
#define CMD59 59
#define CMD59_R R1

/***************************** Application-specific commands ***************/
/* Get the SD card’s status */
#define ACMD13 13
#define ACMD13_R R2
/* Get the number of written write blocks (Minus errors ) */
#define ACMD22 22
#define ACMD22_R R1
/* Set the number of write blocks to be pre-erased before writing */
#define ACMD23 23
#define ACMD23_R R1
/* Get the card’s OCR (SD mode) */
#define ACMD41 41
#define ACMD41_R R1
/* Connect or disconnect the 50kOhm internal pull-up on CD/DAT[3] */
#define ACMD42 42
#define ACMD42_R R1
/* Get the SD configuration register */
#define ACMD51 42
#define ACMD51_R R1


#endif /* LIB_SD_RAW_H_ */
