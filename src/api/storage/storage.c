/*
 * storage.c
 *
 * Created: 31.7.2011 18:19:24
 *  Author: savpek
 */

#include "compiler.h"

#include "./errorh/inc/errorh.h"
#include "./flashmem/inc/flashmem.h"

#include "./storage/config/storage_config.h"
#include "./storage/inc/storage.h"

inline static uint32_t s_true_addr(uint32_t segment_id, uint32_t address)
	{
	return  ((segment_id*STORAGE_SEGMENT_SIZE)+address);
	}

/* Checks that current memory is in range */
static errorc_t s_memory_range_check (uint32_t segment_id, uint32_t address)
	{
	/* Lets check that address is inside of segment */
	if( address > STORAGE_SEGMENT_SIZE )
		{
		return EC_OUT_OF_RANGE;
		}

	/* Lets check that address is inside of defined total memory range */
	if( s_true_addr(segment_id, address) > STORAGE_SIZE )
		{
		return EC_OUT_OF_RANGE;
		}

	return EC_SUCCESS;
	}

/* Reads from selected storage segment! Address is address from beginning of the segment.
 * Returns: EC_STORAGE_END, EC_STORAGE_SEGMENT_END, EC_SUCCESS */
errorc_t storage_read(uint32_t segment_id, uint32_t address, uint8_t *return_byte)
	{
	/* Is in memory range */
	if(s_memory_range_check(segment_id, address) != EC_SUCCESS)
	  	{ return s_memory_range_check(segment_id, address); }

	/* If is in memory range read byte from memory. */
	*return_byte = flashmem_read_uint8_t( s_true_addr(segment_id, address) );

	return EC_SUCCESS;
	}

/* Writes single byte to given address in segment.
 * Returns: EC_STORAGE_NOT_EMPTY, EC_STORAGE_END, EC_SUCCESS, EC_FAILURE */
errorc_t storage_write(uint32_t segment_id, uint32_t address, uint8_t write_byte)
	{
	/* Is in memory range */
	if(s_memory_range_check(segment_id, address) != EC_SUCCESS)
	  	{ return s_memory_range_check(segment_id, address); }

	/* Next we check that is memory empty, this causes only "warning" since
	 * you may want to just zero all bits from memory that you have already written before.
	 * For example, script api uses this functionality */
	uint8_t is_empty_flag = 0;
	if (flashmem_read_uint8_t( s_true_addr(segment_id, address) ) != 0xFF)
		{
		is_empty_flag = 1;
		}

	/* Now we write data */
	flashmem_write_uint8_t( s_true_addr(segment_id, address), write_byte );
	}

/* When you save variables to memory, they are saved with "id's". Id isn't anything else
 * than calculated value from actual address, because every variable contains 4 bytes.
 * So id is simply location of single 32 bit variable from beginning of the memory! */

/* Read uint32_t variable from memory.
 * Returns: EC_SUCCESS, EC_STORAGE_END */
errorc_t storage_read_uint32_t(uint32_t segment_id, uint32_t var_id, uint32_t *var_return)
	{
	/* address is now var_id * 4, since uint32_t contains 4 bytes */
	if(s_memory_range_check(segment_id, var_id * 4) != EC_SUCCESS)
	  	{ return s_memory_range_check(segment_id, var_id * 4); }

	*var_return = flashmem_read_uint32_t( s_true_addr( segment_id, var_id * 4 ) );

	return EC_SUCCESS;
	}

/* Read int32_t variable from memory.
 * Returns: EC_SUCCESS, EC_STORAGE_END */
errorc_t storage_read_int32_t(uint32_t segment_id, uint32_t var_id, int32_t *var_return)
	{
	/* address is now var_id * 4, since uint32_t contains 4 bytes */
	if(s_memory_range_check(segment_id, var_id * 4) != EC_SUCCESS)
	  	{ return s_memory_range_check(segment_id, var_id * 4); }

	*var_return = flashmem_read_int32_t( s_true_addr( segment_id, var_id * 4 ) );

	return EC_SUCCESS;
	}

/* Write uint32_t variable to memory.
 * Returns: EC_SUCCESS, EC_STORAGE_END, EC_STORAGE_NOT_EMPTY. */
errorc_t storage_write_uint32_t(uint32_t segment_id, uint32_t var_id, uint32_t var_write)
	{
	/* address is now var_id * 4, since uint32_t contains 4 bytes */
	if(s_memory_range_check(segment_id, var_id * 4) != EC_SUCCESS)
	  	{ return s_memory_range_check(segment_id, var_id * 4); }

	flashmem_write_uint32_t(s_true_addr(segment_id, var_id * 4), var_write);

	return EC_SUCCESS;
	}

/* Write int32_t variable to memory.
 * Returns: EC_SUCCESS, EC_STORAGE_END, EC_STORAGE_NOT_EMPTY. */
errorc_t storage_write_int32_t(uint32_t segment_id, uint32_t var_id, int32_t var_write)
	{
	/* address is now var_id * 4, since uint32_t contains 4 bytes */
	if(s_memory_range_check(segment_id, var_id * 4) != EC_SUCCESS)
	  	{ return s_memory_range_check(segment_id, var_id * 4); }

	flashmem_write_int32_t(s_true_addr(segment_id, var_id * 4), var_write);

	return EC_SUCCESS;
	}

/* Erases defined segment. */
errorc_t storage_erase_segment(uint32_t segment_id)
	{
	uint32_t block_begin_addr = s_true_addr(segment_id, 0);
	uint32_t block_del_count = 0;

	while(STORAGE_SEGMENT_SIZE > block_del_count*4000)
		{
		flashmem_erase_block(block_begin_addr);
		block_begin_addr += 4000;
		block_del_count++;
		}
	}

/* Delete all data from FLASH IC */
errorc_t storage_erase_all( void )
	{
	flashmem_erase_chip();
	}
