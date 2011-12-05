/*
 * error_codes.h
 *
 * Created: 3.8.2011 12:21:34
 *  Author: savpek
 */ 


#ifndef ERROR_CODES_H_
#define ERROR_CODES_H_

#include "compiler.h"

typedef enum 
	{
	/* Common error types, mostly
	 * used as return codes, not actual errors */
	EC_PASS = 1,
	EC_SUCCESS = 1,
	EC_DONE = 1,
	EC_TRUE = 1,
	EC_NULL = 0,
	EC_FAILURE = 0,
	EC_FALSE = 0,
	EC_FULL = 1,
	EC_NOT_EMPTY = 2,
	EC_LOCKED = 3,
	EC_TIMEOUT = 4,
	EC_OUT_OF_RANGE = 5,
	EC_NOT_NUMBER = 6,
	EC_VALUE_ERROR = 7,
	EC_GOTO_NEXT = 8,
	EC_END = 9,
	EC_EXIT = 10,
	
	/* USART errors. */
	EC_USART_RXD_OVERRUN = 100,
	EC_USART_PARITY_E = 101,
	EC_USART_GENERIC = 102,
	EC_USART_INVALID_INPUT = 103,
	EC_USART_RX_ERROR = 104,
	EC_USART_RX_EMPTY = 105,
	EC_USART_TX_BUSY = 106,
	
	/* FIFO errors */
	EC_FIFO_FULL = 200,
	EC_FIFO_EMPTY = 201,
	EC_FIFO_SUCCES = 202,
	
	/* FLASHMEM errors */
	EC_FLASHMEM_NOT_EMPTY = 300,
	EC_FLASHMEM_LOCKED = 301,
	EC_FLASHMEM_FULL = 302,
	EC_FLASHMEM_NOT_RESPONDING = 303,
	
	/* Storage errors */
	EC_STORAGE_END = 400,
	EC_STORAGE_SEGMENT_END = 401,
	EC_STORAGE_NOT_EMPTY = 402,
	}	errorc_t;

#endif /* ERROR_CODES_H_ */