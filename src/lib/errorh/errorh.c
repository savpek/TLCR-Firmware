/*
 * errorh.c
 *
 * Created: 20.11.2011 11:54:57
 *  Author: savpek
 */

#include "compiler.h"
#include "./errorh/inc/errorh.h"
#include "./print/public/print.h"

/* We store up to 10 different error codes */
static errorc_t error_log[10] = {0};
static uint8_t newest_error_index = 0;

/* This function creates new error to error list. */
void errorh_new_error( errorc_t error_code, error_fatality_t error_fatality )
	{
	/* If error is fatal, hangs here, you can find source of error
	 * by reading with error_code with debugger. */
	if (ERROR_FATAL == error_fatality)
		{
		while(1);
		}

	/* If error is not fatal, add it to error list */
	error_log[newest_error_index%10] = error_code;

	newest_error_index++;
	}

/* Read error, return error code of selected error. Error buffer stores up to 10 errors. */
errorc_t errorh_get_error_code( uint8_t error_number )
	{
	return error_log[error_number%10];
	}

errorc_t errorh_get_newest_error_code()
	{
	return error_log[newest_error_index%10];
	}

void errorh_assert_body( uint32_t assert_value, char* file_name, uint32_t assert_line ) {
	if(assert_value == 0) {
		print_line("");
		print_string("ASSERT! At ");
		print_string(file_name);
		print_string(" on line ");
		print_dec(assert_line);
		print_line(".");
	}
}
