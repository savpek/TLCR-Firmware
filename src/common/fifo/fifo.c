/*
 * fifo.c
 *
 * Created: 20.11.2011 18:50:11
 *  Author: savpek
 */ 

/* ASF */
#include "compiler.h"

/* Own */
#include "./common/fifo/inc/fifo.h"
#include "./error_handler/inc/errorh.h"

static errorc_t is_buffer_full (uint8_t first_index, uint8_t last_index, uint8_t fifo_size)
	{
	if((last_index+1)%fifo_size == first_index%fifo_size)
		{
		return EC_TRUE;	
		}
	else
		{
		return EC_FALSE; 	
		}
	}

static errorc_t is_buffer_empty (uint8_t first_index, uint8_t last_index, uint8_t fifo_size)
	{
	if((first_index+1)%fifo_size == last_index%fifo_size)
		{
		return EC_TRUE;	
		}
	else
		{
		return EC_FALSE;
		}
	}

/* This function limits counters to run between 0 and
 * FIFO_UINTXX_SIZE-1. */
static uint8_t get_next_index_number(uint8_t index_number)
	{
	index_number++;
	
	if(FIFO_UINT8_SIZE <= index_number)
		{
		return 0;
		}
	else
		{
		return index_number;	
		}

	}

/* Gets that how many data cells fifo currently contains. */
extern uint8_t fifo_count_data_uint8( fifo_uint8_t *fifo_data)
	{
	/* In this case, last_index < current_index, happens when
	 * last_index goes over value 0xFF and resets to 0x00 and so on. */
	if(fifo_data->last_index > fifo_data->first_index)
		{
		return 	(uint8_t)(fifo_data->last_index)-(fifo_data->first_index);
		}	
	/* Otherwise current_index > last_index */
	else
		{
		return 	(uint8_t)(FIFO_UINT8_SIZE-fifo_data->first_index)+(fifo_data->last_index);
		}
	}

extern uint8_t fifo_count_data_uint32( fifo_uint32_t *fifo_data)
	{
	/* In this case, last_index < current_index, happens when
	 * last_index goes over value 0xFF and resets to 0x00 and so on. */
	if(fifo_data->last_index < fifo_data->first_index)
		{
		return 	fifo_data->first_index-fifo_data->last_index;
		}	
	/* Otherwise current_index > last_index */
	else
		{
		return 	fifo_data->first_index-fifo_data->last_index;
		}		
	}

/* Adds data to fifo buffer. */
extern errorc_t fifo_put_uint8( fifo_uint8_t *fifo_data, uint8_t new_data)
	{
	/* Checks that buffer is not full */
	if(!is_buffer_full(fifo_data->first_index, fifo_data->last_index, FIFO_UINT8_SIZE))
		{
		fifo_data->data[fifo_data->last_index] = new_data;
		fifo_data->last_index = get_next_index_number(fifo_data->last_index);
		return EC_FIFO_SUCCES;	
		}
	/* If it is full, return error code. */
	else
		{
		return EC_FIFO_FULL;
		}
	}

/* Get data from fifo buffer. */
extern errorc_t fifo_get_uint8( fifo_uint8_t *fifo_data, uint8_t *return_data )
	{
	/* Checks that buffer is not full */
	if(!is_buffer_empty(fifo_data->first_index, fifo_data->last_index, FIFO_UINT8_SIZE))
		{
		fifo_data->first_index = get_next_index_number(fifo_data->first_index);;
		*return_data = fifo_data->data[fifo_data->first_index];
		return EC_SUCCESS;
		}
	/* If it is full, return error code. */
	else
		{
		return EC_FIFO_EMPTY;
		}		
	}
	
extern errorc_t fifo_get_uint32( fifo_uint32_t *fifo_data, uint32_t *return_data )
	{
	/* Checks that buffer is not full */
	if(!is_buffer_empty(fifo_data->first_index, fifo_data->last_index, FIFO_UINT32_SIZE))
		{
		fifo_data->last_index++;
		return fifo_data->data[(fifo_data->last_index-1)%FIFO_UINT8_SIZE];	
		}
	/* If it is full, return error code. */
	else
		{
		return EC_FIFO_EMPTY;
		}	
	}

/* Resets buffer. This must be used every time when
 * new buffer is created. */
extern void fifo_reset_uint8( fifo_uint8_t *fifo_data)
	{
	fifo_data->first_index = 0;
	fifo_data->last_index = 1;
	}

extern void fifo_reset_uint32( fifo_uint32_t *fifo_data)
	{
	fifo_data->first_index = 0;
	fifo_data->last_index = 1;	
	}
