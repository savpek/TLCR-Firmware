/*
 * storage.h
 *
 * Created: 31.7.2011 18:19:10
 *  Author: Savpek Electronics
 *
 * Memory is divided in 8kB segments. Each segment
 * is read by ID. ID is actyally just number which tells where segment
 * starts. For example: segment ID 2 starts in flash addres 8000*2=0x3E80.
 * Segment size is allways 8kB, no matter how large or small it actuyally is.
 */ 

#ifndef STORAGE_H_
#define STORAGE_H_

#include "compiler.h"
#include "./errorh/inc/errorh.h"
#include "./api/storage/config/storage_config.h"
#include "./api/flashmem/inc/flashmem.h"

/* Inits SPI transmission between MCU and FLASH chip */
inline void storage_init(void)
	{
	flashmem_init();	
	}

/* Reads from selected storage segment! Address is address from beginning of the segment.
 * Returns: EC_STORAGE_END, EC_SUCCESS */
extern errorc_t storage_read(uint32_t segment_id, uint32_t address, uint8_t *return_byte);

/* Writes single byte to given address in segment.
 * Returns: EC_STORAGE_NOT_EMPTY, EC_STORAGE_END, EC_SUCCESS, EC_FAILURE */
extern errorc_t storage_write(uint32_t segment_id, uint32_t address, uint8_t write_byte);

/* When you save variables to memory, they are saved with "id's". Id isn't anything else
 * than calculated value from actual address, because every variable contains 4 bytes. 
 * So id is simply location of single 32 bit variable from beginning of the memory! */

/* Read uint32_t variable from memory. 
 * Returns: EC_SUCCESS, EC_STORAGE_END */
extern errorc_t storage_read_uint32_t(uint32_t segment_id, uint32_t var_id, uint32_t *var_return);

/* Read int32_t variable from memory. 
 * Returns: EC_SUCCESS, EC_STORAGE_END */
extern errorc_t storage_read_int32_t(uint32_t segment_id, uint32_t var_id, int32_t *var_return);

/* Write uint32_t variable to memory.
 * Returns: EC_SUCCESS, EC_STORAGE_END, EC_STORAGE_NOT_EMPTY. */
extern errorc_t storage_write_uint32_t(uint32_t segment_id, uint32_t var_id, uint32_t var_write);

/* Write int32_t variable to memory.
 * Returns: EC_SUCCESS, EC_STORAGE_END, EC_STORAGE_NOT_EMPTY. */
extern errorc_t storage_write_int32_t(uint32_t segment_id, uint32_t var_id, int32_t var_write);

/* Erases defined segment. */
extern errorc_t storage_erase_segment(uint32_t segment_id);

/* Erase all data from memory. */
errorc_t storage_erase_all( void );

#endif /* STORAGE_H_ */