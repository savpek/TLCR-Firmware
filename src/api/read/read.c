/*
 * read.c
 *
 * Created: 27.2.2012 21:01:41
 *  Author: savpek
 */

#include "./read/public/read.h"
#include "./read/config/read_config.h"
#include "./print/public/print.h"

static errorc_t is_printable(char c) {
	switch(c) {
		case ' '...('~'+1): /* SPACE - DEL */
		case '\n':
	/*	case '\r':   We don't need these. However, remember echo
					 these with NL char when print linefeed. This
					 because many of terminals require them.		*/
		case '\b':	/* Backspace. */
			return EC_TRUE;
		default:
			return EC_FALSE;
	}
}

char read_char( void ) {
	char c;

	/* Check that received character is PRINTABLE character */
	while(1) {
		c = read_try_get_char();
		if( c != 0) return c;		/* <- If not 0, is printable! */
	}
}

char read_try_get_char( void) {
	char c;

	/* If buffer is empty. */
	if(READ_TRY_GET_CHAR(&c) != EC_SUCCESS) return '\0';

	/* If not printable. */
	if( is_printable(c) != EC_TRUE ) return '\0';

	return c;
}

extern uint8_t read_data( void ) {
	char c;
	while(READ_TRY_GET_CHAR(&c) != EC_SUCCESS );
	return c;
}

extern void read_line( char* return_string, uint8_t return_buffer_size ) {
	char c;
	uint8_t idx = 0;
	while(( c = read_char()) != '\n' )	{		
		if( c == '\b' || c == 0x7F) { /* if BS or DEL */
			if( idx > 0)  {
				idx--;
				print_char(c);
			}			
			return_string[idx] = '\0';
		} 
		else if (idx < return_buffer_size) {
			return_string[idx] = c;
			return_string[idx+1] = '\0';
			idx++;
			print_char(c);
		}
	}
	print_char('\r');
	print_char('\n');
}
