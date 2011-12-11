/*
 * chars_strings.h
 *
 * Created: 20.11.2011 13:06:28
 *  Author: savpek
 */ 


#ifndef CHARS_STRINGS_H_
#define CHARS_STRINGS_H_

#include "compiler.h"
#include "./api/errorh/inc/errorh.h"

/*! TO MAKE SURE THAT RETURN BUFFER IS BIG ENOUGH! */
#define CHRSTR_HEX_U8_SIZE		3
#define CHRSTR_HEX_U32_SIZE		9
#define CHRSTR_DEC_U8_SIZE		4
#define CHRSTR_DEC_U32_SIZE		11
#define CHRSTR_DEC_I32_SIZE		12

/* This function returns string from number. Returns
 * as HEX formatted string.
 * IMPORTANT! : uint8 requires 3 bytes long outptut_str buffer,
 * uint32_t requires 9! This because there should be space for \0 termination. */
extern void chrstr_uint8_to_hex_str(uint8_t input_value, uint8_t *output_str);
extern void chrstr_uint32_to_hex_str(uint32_t input_value, uint8_t *output_str);

/* This function returns string from number. Returns as
 * as intereger formatted string. */
extern void chrstr_uint8_to_dec_str(uint8_t input_value, uint8_t *output_str);
extern void chrstr_uint32_to_dec_str(uint32_t input_value, uint8_t *output_str);
extern void chrstr_int32_to_dec_str(int32_t input_value, uint8_t *output_str);

/* This function compares two strings.
 * Returns: EC_FALSE if they are not same. 
 *			E_TRUE if they are same.
 * Extra end char: As default, string ends to '\0' character, however you can define
 * another char that ends compare there, like space character for example. Define 0 if you don't use. */
extern errorc_t chrstr_string_compare(char *string_one, char *string_two, uint32_t str_cmp_count);

#endif /* CHARS_STRINGS_H_ */