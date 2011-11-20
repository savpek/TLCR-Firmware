/*
 * errorh.h
 *
 * Created: 20.11.2011 11:35:53
 *  Author: savpek
 */ 


#ifndef ERRORH_H_
#define ERRORH_H_

/* Error codes and their short definitions can be found from
 * .error_handler/config/error_codes.h. */
#include "error_codes.h"

/* Error fatalities:
 * FATAL hangs program to infinity inside error loop!
 * NONFATAL simply puts error to error log!
 * WARNING Same as nonfatal, but makes code more readable. */
typedef enum {
	ERROR_FATAL = 1,
	ERROR_NONFATAL = 0,
	ERROR_WARNING = 2,
	} error_fatality_t;

/* This function creates new error to error list. */
extern void errorh_new_error( errorc_t error_code, error_fatality_t error_fatality );

/* Read error, return error code of selected error. Error buffer stores up to 10 errors. */
extern errorc_t errorh_get_error_code( uint8_t error_number );

extern errorc_t errorh_get_newest_error_code( void );

#endif /* ERRORH_H_ */