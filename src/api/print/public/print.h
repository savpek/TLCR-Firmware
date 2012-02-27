/*
 * print.h
 *
 * Created: 23.2.2012 18:58:38
 *  Author: savpek
 */


#ifndef PRINT_H_
#define PRINT_H_

#include "compiler.h"

/*! @brief Prints out character.
 *
 *	@param print_this print this character. */
extern void print_char( char print_this);

/*! @brief Prints print_this string + EOL
 *
 *	@param print_this constant string. */
extern void print_line( char* print_this);

/*! @brief Prints constant string (without EOL).
 *
 *	@param print_this constan string. */
extern void print_string( char* print_this);

/*! @brief Prints uint8_t to output stream, presented as 8bit HEX value.
 *	 For example: 255 -> 0xFF.
 *
 *	@param print_this uint8_t type byte as hex to output stream. */
extern void print_hex8( uint8_t print_this);

/*! @brief Prints uint32_t to output stream, presented as 8bit HEX value.
 *	 For example: ~0 -> 0xFFFFFFFF.
 *
 *	@param print_this uint32_t as hex to output stream. */
extern void print_hex32( uint32_t print_this);

/*! @brief Prints uint8_t type variable bit presentation to output stream.
 *	 For example: 0b00000000
 *
 *	@param print_this uint32_t as bit to output stream. */
extern void print_bit8( uint8_t print_this);

/*! @brief Prints uint8_t type variable bit presentation to output stream.
 *	 For example: 0b00000000....00000
 *
 *	@param print_this uint32_t as bit to output stream. */
extern void print_bit32( uint32_t print_this);

#endif /* PRINT_H_ */