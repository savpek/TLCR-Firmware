/*
 * storage.c
 *
 * Created: 31.7.2011 18:19:24
 *  Author: savpek
 */

#include "compiler.h"
#include "./storage/inc/storage.h"
#include "gpio.h"
#include "spi.h"
#include "./mcu/inc/mcu.h"
#include "./errorh/inc/errorh.h"

#define STORAGE_SPI (&AVR32_SPI)
#define STORAGE_SPI_NPCS 0

#define STORAGE_SEGMENT_SIZE 8000

/* Commands for Atmels AT25DF021 flash chip */
#define STORAGE_FCMD_READ_ARRAY			0x03
#define STORAGE_FCMD_BLOCK_ERASE_4KB	0x20
#define STORAGE_FCMD_BLOCK_ERASE_32KB	0x52
#define STORAGE_FCMD_CHIP_ERASE			0x60
#define STORAGE_FCMD_PROGRAM			0x02
#define STORAGE_FCMD_WRITE_ENABLE		0x06
#define STORAGE_FCMD_WRITE_DISABLE		0x04
#define STORAGE_FCMD_PROTECT_SECTOR		0x36
#define STORAGE_FCMD_UNPROTECT_SECTOR	0x38
#define STORAGE_FCMD_CHECK_PROTECTION	0x3C
#define STORAGE_FCMD_PROG_OTP_SR		0x9B
#define STORAGE_FCMD_READ_OTP_SR		0x77
#define STORAGE_FCMD_READ_STATUS		0x05
#define STORAGE_FCMD_WRITE_STATUS		0x01
#define STORAGE_FCMD_READ_DEV_ID		0x9F

#define STORAGE_FVAL_PROTECT_MEM		0b00111100
#define STORAGE_FVAL_UNPROTECT_MEM		0x00
#define STORAGE_FVAL_DISABLE_SPLR		0x00

/*Dummy byte for SPI */
#define STORAGE_FCMD_DUMMY_WORD			0xFFFF

/*@Set flash IC WEL bit (write enable pit). Required to set before
 * write and erase operations. */
static void _set_wel_bit(void)
	{
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	/* Set WEL bit 1 (Write enable) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_ENABLE);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	}

/*@Get status register, this can be used to check global write
 * protection or by polling device busy bit */
static uint8_t _get_status_register(void)
	{
	uint16_t register_temp = 0;	
	
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);

	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Write command that returns status register data. */
	spi_write(STORAGE_SPI, STORAGE_FCMD_READ_STATUS);
	
	/* Next you have to use SCK clock, then you
	 * receive data in return (device ID) */
	spi_write(STORAGE_SPI, STORAGE_FCMD_DUMMY_WORD );
	spi_read(STORAGE_SPI, &register_temp);

	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	return register_temp;	
	}

/*@Functions to enable, disable and read that is memory
 * write protected. */
static void _disable_write_protect(void)
	{
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
		
	/* Set WEL bit 1 (Write enable) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_ENABLE);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
		
	/* First set SPLR bit to 0 (Sector protection registers locked) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_STATUS);
	spi_write(STORAGE_SPI, STORAGE_FVAL_DISABLE_SPLR);
	spi_write(STORAGE_SPI, STORAGE_FCMD_DUMMY_WORD);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);

	/* Set WEL bit 1 (Write enable) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_ENABLE);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);

	/* Send global write protect disable bits to status register. */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_STATUS);
	spi_write(STORAGE_SPI, STORAGE_FVAL_UNPROTECT_MEM);
	spi_write(STORAGE_SPI, STORAGE_FCMD_DUMMY_WORD);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	}
		
static void _enable_write_protect(void)
	{
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
		
	/* Set WEL bit 1 (Write enable) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_ENABLE);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
		
	/* First set SPLR bit to 0 (Sector protection registers locked) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_STATUS);
	spi_write(STORAGE_SPI, STORAGE_FVAL_DISABLE_SPLR);
	spi_write(STORAGE_SPI, STORAGE_FCMD_DUMMY_WORD);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Set WEL bit 1 (Write enable) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_ENABLE);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);

	/* Send global write protect disable bits to status register. */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_STATUS);
	spi_write(STORAGE_SPI, STORAGE_FVAL_PROTECT_MEM);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	}

/*@Sends read command to flash chips, and set it to correct
 * addres where reading should begin. */
static void _read_start(uint32_t flash_addr)
	{
	/* For make sure chip is unselected before start read
	 * routines */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
		
	/* Select chip and send command */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_READ_ARRAY);	
	
	/* Next you have to send 3 addres bytes, MSBit first. */
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>16)));
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>8)));
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>0)));
	
	/* After this you can read from memory, remember end
	 * read array command with _storage_read_stop() function!.
	 * This deselect chip and end read command. */
	}

/*@ Returns next byte from flash memory. Because reading is
 * one operation which starts in certain point, you don't need
 * specify addres again, chip returns stream of data. 
 * Remember to end with _storage_stream_stop command! */
static uint8_t _read_next_byte(void)
	{
	uint16_t byte_temp;
	/* Send one dummy byte to receive data */
	spi_write(STORAGE_SPI, STORAGE_FCMD_DUMMY_WORD);
	
	/* Reads data from SPI register */
	spi_read(STORAGE_SPI, &byte_temp);
	
	return (uint8_t)byte_temp;
	}

/*@ Ends stream reading mode. */
static void _read_stop(void)
	{
	/* Ends read command by deselecting chip */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);	
	}

/*@Reads one byte from given addres at flash memory. */
static uint8_t _get_byte(uint32_t flash_addr)
	{
	uint16_t readed_byte;
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Select flash chip (#CS pin down) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Send read array command to chip */
	spi_write(STORAGE_SPI, STORAGE_FCMD_READ_ARRAY);
	
	/* Next you have to send 3 addres bytes, MSBit first. */
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>16)));
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>8)));
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>0)));

	/* Send one dummy byte to receive data */
	spi_write(STORAGE_SPI, STORAGE_FCMD_DUMMY_WORD);
	
	/* Reads data from SPI register */
	spi_read(STORAGE_SPI, &readed_byte);
	
	/* Deselect flash chip (#CS pin up) */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	return readed_byte;
	}

static void _put_byte(uint32_t flash_addr, uint8_t data)
	{
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	_disable_write_protect();
		
	/* Set WEL bit 1 (Write enable) */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	spi_write(STORAGE_SPI, STORAGE_FCMD_WRITE_ENABLE);
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	/* Send write command to chip */
	spi_write(STORAGE_SPI, STORAGE_FCMD_PROGRAM);
	
	/* Next you have to send 3 addres bytes, MSBit first. */
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>16)));
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>8)));
	spi_write(STORAGE_SPI, ((uint16_t)(flash_addr>>0)));

	/* Send data which is going to be written in memory. */
	spi_write(STORAGE_SPI, data);
	
	/* Deselect flash chip (#CS pin up) */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	_enable_write_protect();
	}

/*@ Init storage, basically init SPI
 * transmission between flash chip and MCU. */
extern void storage_init(void)
	{
	/* IO map for SPI */
	static const gpio_map_t SPI_GPIO_MAP =
		{
		{AVR32_SPI_SCK_0_0_PIN,		AVR32_SPI_SCK_0_0_FUNCTION         },  // SPI Clock.
		{AVR32_SPI_MISO_0_0_PIN,    AVR32_SPI_MISO_0_0_FUNCTION        },  // MISO.
		{AVR32_SPI_MOSI_0_0_PIN,    AVR32_SPI_MOSI_0_0_FUNCTION        },  // MOSI.
		{AVR32_SPI_NPCS_0_0_PIN,	AVR32_SPI_NPCS_0_0_FUNCTION}  // Chip Select NPCS.
		};

	/* Setting for SPI */
	spi_options_t spiOptions =
		{
		.reg          = 0,
		.baudrate     = 1200000,
		.bits         = 8,
		.spck_delay   = 30,
		.trans_delay  = 30,
		.stay_act     = 1,
		.spi_mode     = 0,
		.modfdis      = 1
		};

	/* IO mapping for SPI */
	gpio_enable_module(SPI_GPIO_MAP,
					 sizeof(SPI_GPIO_MAP) / sizeof(SPI_GPIO_MAP[0]));

	/* MCU is master */
	spi_initMaster(STORAGE_SPI, &spiOptions);

	/* Set selection mode: variable_ps, pcs_decode, delay. */
	spi_selectionMode(STORAGE_SPI, 0, 0, 0);

	/* Enable SPI. */
	spi_enable(STORAGE_SPI);
	
	spi_setupChipReg(STORAGE_SPI, &spiOptions, MCU_PBA_F);
	}

/*@Reads one uint32_t variable from memory. You need to know
 * segment and variable ID to read it from memory. Returns value
 * of variable from function.*/
extern uint32_t storage_get_variable(uint16_t segment_id, uint16_t variable_id) 
	{
	uint32_t ret_variable = 0;	
	_read_start((segment_id*STORAGE_SEGMENT_SIZE)+(variable_id*4));
	
	ret_variable |= _read_next_byte()<<24;
	ret_variable |= _read_next_byte()<<16;
	ret_variable |= _read_next_byte()<<8;
	ret_variable |= _read_next_byte()<<0;
	
	_read_stop();
	
	return ret_variable;
	}		

/*@ Write data to defined segment. */
extern errorc_t storage_write_segment(uint8_t segment_id, uint16_t next_byte_id, uint8_t data)
	{		
	/* Check that written data is in range of segment size */
	if (next_byte_id > STORAGE_SEGMENT_SIZE)
		{
		return EC_OUT_OF_RANGE;	
		}
		
	/* Waits until previous write operation is done (busy flag is 0 from
	 * flash chips status register */
	while((_get_status_register()&0x01) != 0);
	
	/* Checks that current target write target has value 0xFF (empty) */
	if(_get_byte(segment_id*STORAGE_SEGMENT_SIZE+next_byte_id) != 0xFF)
		{
		return EC_NOT_EMPTY;
		}
	
	/* Finally save data to memory */	
	_put_byte(segment_id*STORAGE_SEGMENT_SIZE+next_byte_id, data);
	
	return EC_DONE;
	}

/*@ Reads data from pointed segment. Returns one byte from memory. */
extern errorc_t storage_read_segment(uint8_t segment_id, uint16_t next_byte_id, uint8_t* ret_data)
	{
	/* Set static value to 0xFFFFFFF, this next byte addres cannot be.
	 * First time this function will be runned, will program begin reading
	 * at new point because next_addr cannot be prev_addr if its 0xFFFFFFFF */
	static uint32_t prev_flash_addr = 0xFFFFFFFF;
	uint32_t next_flash_addr = segment_id*STORAGE_SEGMENT_SIZE+next_byte_id;
	
	/* Check that you are reading segment you ment to */
	if (next_byte_id > STORAGE_SEGMENT_SIZE)
		{
		return EC_OUT_OF_RANGE;	
		}
	
	/* Check that is previous read byte one addr before next, if not
	 * we should start flash chip reading from new location. */
	if (prev_flash_addr+1 != next_flash_addr)
		{
		spi_unselectChip(STORAGE_SPI, 0);
		_read_start(next_flash_addr);
		}
	
	*ret_data = _read_next_byte();
	
	prev_flash_addr = next_flash_addr;
	
	return EC_DONE;
	}

/*@Gets flash id. This is tests that flash chip is
 * working correctly. */
extern uint16_t storage_get_flash_id ()
	{	
	uint16_t id_temp = 0;
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Get #CS pin down (Flash chip active). */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Write command that returns flash id */
	spi_write(STORAGE_SPI, STORAGE_FCMD_READ_DEV_ID);
	
	/* Next you have to use SCK clock, then you
	 * receive data in return (device ID) */
	spi_write(STORAGE_SPI, 0xFF );
	spi_read(STORAGE_SPI, &id_temp);

	/* Get #CS pin up (Flash chip disabled). */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	return id_temp;
	}

/* @This function erases entire chip. Stays in function as
 *	long as erasing takes (flash IC stays busy). */
extern errorc_t storage_erase_all(void)
	{
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	_disable_write_protect();
	_set_wel_bit();
	
	/* Get #CS pin down (Flash chip active). */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	/* Write command that erases entire chip */
	spi_write(STORAGE_SPI, STORAGE_FCMD_CHIP_ERASE);
	
	/* Get #CS pin up (Flash chip disabled). */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	
	/* Get #CS pin down (Flash chip active). */
	spi_selectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	/* Waits until chip erase is done. */
	while((_get_status_register()&0x01) != 0);
	
	/* Get #CS pin up (Flash chip disabled). */
	spi_unselectChip(STORAGE_SPI, STORAGE_SPI_NPCS);
	
	return EC_TRUE;	
	}