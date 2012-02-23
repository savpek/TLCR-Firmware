/*
 * print.h
 *
 * Created: 23.2.2012 18:58:38
 *  Author: savpek
 */


#ifndef PRINT_H_
#define PRINT_H_

/*! @brief Prints out character.
 *
 *	@param print_this print this character. */
void print_char( char print_this);

/*! @brief Prints print_this string + EOL
 *
 *	@param print_this constant string. */
void print_line( const char* print_this);

/*! @brief Prints non constant string with EOL
 *
 *	@param print_this non constant string. */
void print_var_line( char* print_this);

/*! @brief Prints constant string (without EOL).
 *
 *	@param print_this non constan string. */
void print_string( const char* print_this);

/*! @brief Prints constant string (without EOL).
 *
 *	@param print_this non constan string. */
void print_string( const char* print_this);

void print_hex8( uint8_t print_this);

void print_hex32( uint32_t print_this);

void print_bit8( uint8_t print_this);

void print_bit32( uint32_t print_this);

#endif /* PRINT_H_ */