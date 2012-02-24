/*
 * read.h
 *
 * Created: 23.2.2012 19:08:29
 *  Author: savpek
 */


#ifndef READ_H_
#define READ_H_

#include "compiler.h"

/*! @brief Read character from defined stream. Wait until get one.
 *
 *	 Only accepts visible characters from ASCII table.
 *
 *	@return character. */
extern char read_char( void);

/*! @brief Read data from defined stream. Wait until get something. 
 *
 *	 Accepts any value as data. Doesn't filter input.
 *	 
 *	@return data byte from input buffer. */
extern uint8_t read_data( void);

/*! @brief Read character from defined stream. Don't stop if theres none. If no
 *	 character currently waiting returns NULL!.
 *
 *	 Ignores non printable characters.
 *
 *	@return character or NULL!*/
extern char read_try_get_char( void);

/*! @brief Read line from stream. If line buffer get full, extra
 *	 characters are silently ignored.
 *
 *	 How backspace and DEL characters work: they remove characters
 *	 from string buffer. Echoes BS/DEL only when string is non empty.
 *	 Basically emulates "terminal" behavior.
 *
 *	@param[out] return_string[] this is string. */
extern void read_line( char* return_string, uint8_t flags);

#endif /* READ_H_ */