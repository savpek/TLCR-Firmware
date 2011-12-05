/*
 * fifo.h
 *
 * Created: 20.11.2011 18:50:01
 *  Author: savpek
 */ 


#ifndef FIFO_H_
#define FIFO_H_

/* ASF */
#include "compiler.h"

/* Internal */
#include "./common/fifo/config/fifo_config.h"

/* TLCR Libs */
#include "./api/errorh/inc/errorh.h"

typedef struct {
	uint8_t first_index;
	uint8_t last_index;
	uint8_t data[FIFO_UINT8_SIZE];
} fifo_uint8_t;

typedef struct {
	uint8_t first_index;
	uint8_t last_index;
	uint32_t data[FIFO_UINT32_SIZE];	
} fifo_uint32_t;

/* Returns count of data cells currently in fifo. */
extern uint8_t fifo_count_data_uint8( fifo_uint8_t *fifo_data);
extern uint8_t fifo_count_data_uint32( fifo_uint32_t *fifo_data);

/* Adds data to fifo buffer. */
extern errorc_t fifo_put_uint8( fifo_uint8_t *fifo_data, uint8_t new_data);
extern errorc_t fifo_put_uint32( fifo_uint32_t *fifo_data, uint32_t new_data);

/* Get data from fifo buffer. */
extern errorc_t fifo_get_uint8( fifo_uint8_t *fifo_data, uint8_t *return_data );
extern errorc_t fifo_get_uint32( fifo_uint32_t *fifo_data, uint32_t *return_data );

/* Resets buffer, clear all indexes. */
extern void fifo_reset_uint8( fifo_uint8_t *fifo_data);
extern void fifo_reset_uint32( fifo_uint32_t *fifo_data);

#endif /* FIFO_H_ */