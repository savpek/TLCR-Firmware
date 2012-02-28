/*
 * read_config.h
 *
 * Created: 28.2.2012 0:17:43
 *  Author: savpek
 */ 


#ifndef READ_CONFIG_H_
#define READ_CONFIG_H_

/*! This version of READ api implementation requires following
 *	function from stream driver:
 *	errorc_t READ_TRY_GET_CHAR(char *a) 
 *
 *	Function must return EC_SUCCESS (or equal) when it
 *	succesfully returns character from stream. */

#include "./usart/public/usart.h"

/*! Defines read from stream function. */
#define READ_TRY_GET_CHAR(a) usart_try_read_char(a)

#endif /* READ_CONFIG_H_ */