/*
 * flashmem.h
 *
 * Created: 3.12.2011 12:28:30
 *  Author: savpek
 */ 


#ifndef FLASHMEM_H_
#define FLASHMEM_H_

#include "compiler.h"
#include "./api/flashmem/config/flashmem_config.h"
#include "./api/errorh/inc/errorh.h"

/* This function inits currently used flash IC and MCU IO */
extern void flashmem_init(void);

/* Returns EC_SUCCESS/EC_FAILURE based on that that did we receive respond from
 * flash IC. */
extern errorc_t flashmem_test( void );

/* Reads one uint8_t type variable from memory (character for example) */
extern uint8_t flashmem_read_uint8_t(uint32_t address);

/* Reads one uint32_t type variable from memory (defined value for example) */
extern uint32_t flashmem_read_uint32_t(uint32_t address);

/* Reads one int32_t type variable from memory (defined value for example) */
extern int32_t flashmem_read_int32_t(uint32_t address);

/* Write one byte to memory */
extern void flashmem_write_uint8_t(uint32_t address, uint8_t data);

/* Write uint32_t type of data to memory */
extern void flashmem_write_uint32_t(uint32_t address, uint32_t data);

/* Write int32_t type of data to memory */
extern void flashmem_write_int32_t(uint32_t address, int32_t data);

/* Erase block that begins from given address. Polling flash IC busy state
 * as long as it's done. */
extern void flashmem_erase_block(uint32_t address);

/* Erase hole chip, stays inside of function as long as erase will take. */
extern void flashmem_erase_chip( void );

#endif /* FLASHMEM_H_ */