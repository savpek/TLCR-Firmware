/*
 * flashmem_config.h
 *
 * Created: 3.12.2011 12:54:33
 *  Author: savpek
 */ 


#ifndef FLASHMEM_CONFIG_H_
#define FLASHMEM_CONFIG_H_

#include "compiler.h"

/* Define to enter debugmode */
#define FLASHMEM_DEBUG

/* Pin configuration for SPI interface */
#define FLASHMEM_SCK_PIN			AVR32_SPI_SCK_0_0_PIN
#define FLASHMEM_SCK_FUNCTION		AVR32_SPI_SCK_0_0_FUNCTION
#define FLASHMEM_MISO_PIN			AVR32_SPI_MISO_0_0_PIN
#define FLASHMEM_MISO_FUNCTION		AVR32_SPI_MISO_0_0_FUNCTION
#define FLASHMEM_MOSI_PIN			AVR32_SPI_MOSI_0_0_PIN
#define FLASHMEM_MOSI_FUNCTION		AVR32_SPI_MOSI_0_0_FUNCTION
#define FLASHMEM_NPCS_PIN			AVR32_SPI_NPCS_0_0_PIN
#define FLASHMEM_NPCS_FUNCTION		AVR32_SPI_NPCS_0_0_FUNCTION

/* Settings for SPI */
/* Points to used SPI ->*/
#define FLASHMEM_SPI				(&AVR32_SPI)
/* Which NPCS channel flash IC is connected -> */
#define FLASHMEM_SPI_CHANNEL		0
#define FLASHMEM_SPI_SPEED			1200000
#define FLASHMEM_SPI_CHAR_LENGTH	8
/* Delay before first clock pulse after selecting slave (in PBA clock periods) -> */
#define FLASHMEM_SPI_SPCK_DELAY		30
/* Delay between each transfer/character (in PBA clock periods) -> */
#define FLASHMEM_SPI_TRANS_DELAY	30
/* Sets this chip to stay active after last transfer to it -> */
#define FLASHMEM_SPI_STAY_ACT		1
/* SPI mode is set as master, not defined here! */


/* Commands for Atmels AT25DF021 flash chip */
#define FLASHMEM_CMD_READ_ARRAY			0x03
#define FLASHMEM_CMD_BLOCK_ERASE_4KB	0x20
#define FLASHMEM_CMD_BLOCK_ERASE_32KB	0x52
#define FLASHMEM_CMD_CHIP_ERASE			0x60
#define FLASHMEM_CMD_PROGRAM			0x02
#define FLASHMEM_CMD_WRITE_ENABLE		0x06
#define FLASHMEM_CMD_WRITE_DISABLE		0x04
#define FLASHMEM_CMD_PROTECT_SECTOR		0x36
#define FLASHMEM_CMD_UNPROTECT_SECTOR	0x38
#define FLASHMEM_CMD_CHECK_PROTECTION	0x3C
#define FLASHMEM_CMD_PROG_OTP_SR		0x9B
#define FLASHMEM_CMD_READ_OTP_SR		0x77
#define FLASHMEM_CMD_READ_STATUS		0x05
#define FLASHMEM_CMD_WRITE_STATUS		0x01
#define FLASHMEM_CMD_READ_DEV_ID		0x9F

#define FLASHMEM_VALUE_PROTECT_MEM		0b00111100
#define FLASHMEM_VALUE_UNPROTECT_MEM	0x00
#define FLASHMEM_VALUE_DISABLE_SPLR		0x00

/* Masks for reading statusbits */
#define FLASHMEM_STATUS_BUSY_MASK		0b00000001

/*Dummy byte for SPI */
#define FLASHMEM_CMD_DUMMY				0xFFFF

#endif /* FLASHMEM_CONFIG_H_ */