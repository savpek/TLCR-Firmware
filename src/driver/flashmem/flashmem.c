/*
 * flashmem.c
 *
 * Created: 3.12.2011 13:24:04
 *  Author: savpek
 */

/* ASF */
#include "compiler.h"
#include "spi.h"
#include "./gpio/public/gpio.h"

/* Internal */
#include "./flashmem/inc/flashmem.h"
#include "./flashmem/config/flashmem_config.h"
#include "./mcu/inc/mcu.h"

/* TLCR lib */
#include "./errorh/inc/errorh.h"

/* Routine for select FLASH IC (#CS pin to low) */
static void s_npcs_select( void )
	{
	#ifdef FLASHMEM_DEBUG
	if(SPI_ERROR_ARGUMENT == spi_selectChip(FLASHMEM_SPI, FLASHMEM_SPI_CHANNEL))
		{
		errorh_print("\n\rNPCS_SELECT_ERROR\n\r");
		}
	#else
	spi_selectChip(FLASHMEM_SPI, FLASHMEM_SPI_CHANNEL);
	#endif
	}

/* Routine for unselect FLASH IC (#CS pin to high) */
static void s_npcs_unselect( void )
	{
	#ifdef FLASHMEM_DEBUG
	if(SPI_ERROR_TIMEOUT == spi_unselectChip(FLASHMEM_SPI, FLASHMEM_SPI_CHANNEL))
		{
		errorh_print("\n\rNPCS_UNSELECT_ERROR\n\r");
		}
	#else
	spi_unselectChip(FLASHMEM_SPI, FLASHMEM_SPI_CHANNEL);
	#endif
	}

/* Send one 8bit command via SPI interface */
static void s_send_cmd(uint8_t command)
	{
	#ifdef FLASHMEM_DEBUG
	if( SPI_ERROR_TIMEOUT == spi_write(FLASHMEM_SPI, command) )
		{
		errorh_print("\n\rFLASHMEM_WRITE_ERROR");
		}
	#else
	spi_write(FLASHMEM_SPI, command);
	#endif
	}

/* Reads current SPI receive register */
static uint8_t s_read()
	{
	uint16_t return_temp = 0;

	#ifdef FLASHMEM_DEBUG
	if(spi_read(FLASHMEM_SPI, &return_temp))
		{
		errorh_print("\n\rFLASHMEM_READ_ERROR\n\r");
		}
	#else
	spi_read(FLASHMEM_SPI, &return_temp);
	#endif

	return (uint8_t)return_temp;
	}

static uint8_t s_get_status_register(void)
	{
	uint8_t register_temp = 0;

	s_npcs_select();

	/* Write command that returns status register data. */
	s_send_cmd(FLASHMEM_CMD_READ_STATUS);

	/* Next you have to use SCK clock, then you
	 * receive data in return (device ID) */
	s_send_cmd( FLASHMEM_CMD_DUMMY );

	/* Get received answer from statusregister. */
	register_temp = s_read();

	s_npcs_unselect();

	return register_temp;
	}

static void s_wait_for_ic_ready()
	{
	/* Loops until flash IC returns "not busy" register value. */
	while((s_get_status_register()&FLASHMEM_STATUS_BUSY_MASK) != 0);
	}

/* Set flash IC WEL bit (write enable pit). Required to set before
 * write and erase operations. */
static void s_set_wel_bit(void)
	{
	/* Set WEL bit 1 (Write enable) */
	s_npcs_select();
	spi_write(FLASHMEM_SPI, FLASHMEM_CMD_WRITE_ENABLE);
	s_npcs_unselect();
	}

static void s_enable_write_protect(void)
	{
	/* Write enable */
	s_set_wel_bit();

	/* First set SPLR bit to 0 (Sector protection registers locked) */
	s_npcs_select();
	s_send_cmd(FLASHMEM_CMD_WRITE_STATUS);
	s_send_cmd(FLASHMEM_VALUE_DISABLE_SPLR);
	s_send_cmd(FLASHMEM_CMD_DUMMY);
	s_npcs_unselect();

	/* Write enable */
	s_set_wel_bit();

	/* Send global write protect disable bits to status register. */
	s_npcs_select();
	s_send_cmd(FLASHMEM_CMD_WRITE_STATUS);
	s_send_cmd(FLASHMEM_VALUE_PROTECT_MEM);
	s_npcs_unselect();
	}

static void s_disable_write_protect(void)
	{
	/* Set WEL bit 1 (Write enable) */
	s_set_wel_bit();

	/* First set SPLR bit to 0 (Sector protection registers locked) */
	s_npcs_select();
	s_send_cmd(FLASHMEM_CMD_WRITE_STATUS);
	s_send_cmd(FLASHMEM_VALUE_DISABLE_SPLR);
	s_send_cmd(FLASHMEM_CMD_DUMMY);
	s_npcs_unselect();

	/* Set WEL bit 1 (Write enable) */
	s_set_wel_bit();

	/* Send global write protect disable bits to status register. */
	s_npcs_select();
	s_send_cmd(FLASHMEM_CMD_WRITE_STATUS);
	s_send_cmd(FLASHMEM_VALUE_UNPROTECT_MEM);
	s_send_cmd(FLASHMEM_CMD_DUMMY);
	s_npcs_unselect();
	}

/* This function inits currently used flash IC and MCU IO */
void flashmem_init(void)
	{
	/* IO map for SPI */
	static const gpio_map_t SPI_GPIO_MAP =
		{
		{FLASHMEM_SCK_PIN,			FLASHMEM_SCK_FUNCTION   },
		{FLASHMEM_MISO_PIN,			FLASHMEM_MISO_FUNCTION  },
		{FLASHMEM_MOSI_PIN,			FLASHMEM_MOSI_FUNCTION  },
		{FLASHMEM_NPCS_PIN,			FLASHMEM_NPCS_FUNCTION	}
		};

	/* Setting for SPI */
	spi_options_t spiOptions =
		{
		.reg          = 0,
		.baudrate     = 1200000,
		.bits         = 8,
		.spck_delay   = FLASHMEM_SPI_SPCK_DELAY,
		.trans_delay  = FLASHMEM_SPI_TRANS_DELAY,
		.stay_act     = FLASHMEM_SPI_STAY_ACT,
		.spi_mode     = 0,
		.modfdis      = 1
		};

	/* IO mapping for SPI */
	gpio_enable_module(SPI_GPIO_MAP,
					 sizeof(SPI_GPIO_MAP) / sizeof(SPI_GPIO_MAP[0]));

	/* MCU is master */
	spi_initMaster(FLASHMEM_SPI, &spiOptions);

	/* Set selection mode: variable_ps, pcs_decode, delay. */
	spi_selectionMode(FLASHMEM_SPI, 0, 0, 0);

	/* Enable SPI. */
	spi_enable(FLASHMEM_SPI);

	spi_setupChipReg(FLASHMEM_SPI, &spiOptions, MCU_PBA_F);

	/* If debug is enabled, we print error message if IC isn't responding. */
	#ifdef FLASHMEM_DEBUG
	if(EC_FAILURE == flashmem_test())
		{
		errorh_print("\r\nINIT: FLASHMEM_TEST_FAIL\n\r");
		}
	#endif
	}

/* Returns EC_SUCCESS/EC_FAILURE based on that that did we receive respond from
 * flash IC. */
errorc_t flashmem_test( void )
	{
	uint16_t id_temp = 0;

	s_npcs_select();

	/* Write command that returns flash id */
	s_send_cmd(FLASHMEM_CMD_READ_DEV_ID);

	/* Next send dummy byte, dummy byte moves chain 8 bits forward
	 * so you can read received bits from SPI receive register. */
	s_send_cmd(FLASHMEM_CMD_DUMMY);

	/* Read data from SPI register */
	id_temp = s_read();

	/* Get #CS pin up (Flash chip disabled). */
	s_npcs_unselect();

	/* 0xFF or 0x00 means no ID received. */
	if(0xFF > id_temp && 0x00 < id_temp)
		{
		return EC_SUCCESS;
		}
	else
		{
		return EC_FAILURE;
		}
	}

/* Reads one uint8_t type variable from memory (character for example) */
uint8_t flashmem_read_uint8_t(uint32_t address )
	{
	uint8_t return_temp;

	/* Select flash chip (#CS pin down) */
	s_npcs_select();

	/* Send read array command to chip */
	s_send_cmd(FLASHMEM_CMD_READ_ARRAY);

	/* Next you have to send 3 addres bytes, MSBit first. */
	s_send_cmd((uint8_t)(address>>16));
	s_send_cmd((uint8_t)(address>>8));
	s_send_cmd((uint8_t)(address>>0));

	/* Send one dummy byte to receive data */
	s_send_cmd(FLASHMEM_CMD_DUMMY);

	/* Reads data from SPI register */
	return_temp = s_read();

	/* Deselect flash chip (#CS pin up) */
	s_npcs_unselect();

	return return_temp;
	}

/* Reads one uint32_t type variable from memory (defined value for example) */
uint32_t flashmem_read_uint32_t(uint32_t address)
	{
	uint32_t return_temp = 0;
	return_temp |= (uint32_t)(flashmem_read_uint8_t(address))<<24;
	return_temp |= (uint32_t)(flashmem_read_uint8_t(address+1))<<16;
	return_temp |= (uint32_t)(flashmem_read_uint8_t(address+2))<<8;
	return_temp |= (uint32_t)(flashmem_read_uint8_t(address+3))<<0;
	return return_temp;
	}

/* Reads one int32_t type variable from memory (defined value for example) */
int32_t flashmem_read_int32_t(uint32_t address)
	{
	int32_t return_temp = 0;
	return_temp |= (int32_t)(flashmem_read_uint8_t(address))<<24;
	return_temp |= (int32_t)(flashmem_read_uint8_t(address+1))<<16;
	return_temp |= (int32_t)(flashmem_read_uint8_t(address+2))<<8;
	return_temp |= (int32_t)(flashmem_read_uint8_t(address+3))<<0;
	return return_temp;
	}

/* Write one byte to memory */
void flashmem_write_uint8_t(uint32_t address, uint8_t data)
	{
	s_disable_write_protect();

	/* Set WEL bit 1 (Write enable) */
	s_set_wel_bit();

	s_npcs_select();
	/* Send write command to chip */
	s_send_cmd(FLASHMEM_CMD_PROGRAM);

	/* Next you have to send 3 addres bytes, MSBit first. */
	s_send_cmd((uint8_t)(address>>16));
	s_send_cmd((uint8_t)(address>>8));
	s_send_cmd((uint8_t)(address>>0));

	/* Send data which is going to be written in memory. */
	s_send_cmd(data);

	/* Deselect flash chip (#CS pin up) */
	s_npcs_unselect();

	s_enable_write_protect();

	/* Wait until write is done. */
	s_wait_for_ic_ready();
	}

/* Write uint32_t type of data to memory */
void flashmem_write_uint32_t(uint32_t address, uint32_t data)
	{
	flashmem_write_uint8_t(address, data>>24);
	flashmem_write_uint8_t(address+1, data>>16);
	flashmem_write_uint8_t(address+2, data>>8);
	flashmem_write_uint8_t(address+3, data>>0);
	}

/* Write int32_t type of data to memory */
void flashmem_write_int32_t(uint32_t address, int32_t data)
	{
	flashmem_write_uint8_t(address, data>>24);
	flashmem_write_uint8_t(address+1, data>>16);
	flashmem_write_uint8_t(address+2, data>>8);
	flashmem_write_uint8_t(address+3, data>>0);
	}

/* Erases 4kb block, block begins from given addr */
void flashmem_erase_block(uint32_t address)
	{
	s_disable_write_protect();
	s_set_wel_bit();

	s_npcs_select();

	s_send_cmd(FLASHMEM_CMD_BLOCK_ERASE_4KB);

	s_send_cmd((uint8_t)(address>>16));
	s_send_cmd((uint8_t)(address>>8));
	s_send_cmd((uint8_t)(address>>0));

	s_npcs_unselect();

	/* Wait until erase is done. */
	s_wait_for_ic_ready();
	}

/* Erase hole chip, stays inside of function as long as erase will take. */
void flashmem_erase_chip( void )
	{
	s_disable_write_protect();
	s_set_wel_bit();

	/* Get #CS pin down (Flash chip active). */
	s_npcs_select();

	/* Write command that erases entire chip */
	s_send_cmd(FLASHMEM_CMD_CHIP_ERASE);

	/* Get #CS pin up (Flash chip disabled). */
	s_npcs_unselect();

	/* Loops here until chip is erased. */
	s_wait_for_ic_ready();
	}

/* Function gets begin address of page */
void flashmem_get_page_addr ( uint32_t page_number )
	{

	}
