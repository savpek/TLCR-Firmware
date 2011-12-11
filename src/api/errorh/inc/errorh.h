/*
 * errorh.h
 *
 * Created: 20.11.2011 11:35:53
 *  Author: savpek
 */ 


#ifndef ERRORH_H_
#define ERRORH_H_

/* Error codes and their short definitions can be found from
 * .errorh/config/error_codes.h. */
#include "./api/errorh/config/error_codes.h"

/* Error fatalities:
 * FATAL hangs program to infinity inside error loop!
 * NONFATAL simply puts error to error log!
 * WARNING Same as nonfatal, but makes code more readable. */
typedef enum {
	ERROR_FATAL = 1,
	ERROR_NONFATAL = 0,
	ERROR_WARNING = 2,
	} error_fatality_t;

/* Prints error to output stream, you can define this pointer to point
 * different kind of streams (usart, lcd for example) */
void (*errorh_print)(char *error_str);

#define ERRORH_PRINT(error_msg) \
	errorh_print("\r\n");\
	errorh_print(error_msg); \
	errorh_print("\r\n");\
	errorh_print(__FILE__)

/* This sets error handler used in project. For example, to set error messages
 * printed to usart use: errorh_set_print_handler(usart_write_line); */
#define ERRORH_SET_PRINT_HANDLER(x) errorh_print = x

/* This function creates new error to error list. */
extern void errorh_new_error( errorc_t error_code, error_fatality_t error_fatality );

/* Read error, return error code of selected error. Error buffer stores up to 10 errors. */
extern errorc_t errorh_get_error_code( uint8_t error_number );

extern errorc_t errorh_get_newest_error_code( void );

#endif /* ERRORH_H_ */