/*
 * print.c
 *
 * Created: 24.2.2012 22:17:41
 *  Author: savpek
 */

#include "./print/public/print.h"
#include "./print/config/print_config.h"

#include "./str/public/str.h"

#include "compiler.h"

static void print_eol() {
	PRINT_CHAR_STREAMF('\n');
	PRINT_CHAR_STREAMF('\r');
}

void print_char( char print_this ) {
	PRINT_CHAR_STREAMF(print_this);
}

void print_line( char* print_this ) {
	print_string(print_this);
	print_eol();
}

void print_string( char* print_this )
{
	uint8_t idx = 0;
	while(print_this[idx] != 0) {
		PRINT_CHAR_STREAMF(print_this[idx]);
		idx++;
	}
}

void print_hex8( uint8_t print_this_value )
{
	char output_buffer[STR_HEX_UINT8_SIZE];
	str_from_uint8_hex(print_this_value, output_buffer);
	print_string(output_buffer);
}

void print_hex32( uint32_t print_this_value )
{
	char output_buffer[STR_HEX_UINT32_SIZE];
	str_from_uint32_hex(print_this_value, output_buffer);
	print_string(output_buffer);
}
