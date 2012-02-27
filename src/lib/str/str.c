/*
 * chars_strings.c
 *
 * Created: 20.11.2011 13:06:46
 *  Author: savpek
 */

#include "compiler.h"

#include "./str/public/str.h"
#include "./errorh/inc/errorh.h"

static void s_return_hex(uint32_t int_value, uint8_t *ret_char)
	{
	/* First we mask off rest than last 4 bits (which counts as one
	 * hex character */
	int_value = int_value &	0x0000000F;

	/* If value is number */
	if(int_value >= 0 && int_value <= 9)
		{
		*ret_char = int_value+'0';
		}
	/* If value is HEX */
	else
		{
		*ret_char = int_value+'A'-10;
		}
	}

static void return_string_from_uint(uint32_t input_value, char *output_str, uint32_t divider, uint8_t str_begin_idx) {
	uint8_t number_idx = 0;
	
	do
		{
		if(( input_value / divider ) != 0 ||
			number_idx > 0)
			{
			output_str[str_begin_idx] = ( input_value / divider )+'0';
			str_begin_idx++;
			number_idx++;
			}

		input_value = input_value%divider;
		divider = divider/10;

		} while ( divider != 0 );

	/* This is for special case: value was 0. */
	if(number_idx == 0) {
		output_str[str_begin_idx] = '0';
		str_begin_idx++;
	}

	output_str[str_begin_idx] = '\0';
}

void str_merge(char *source, char *target, uint8_t begin_idx ) {
	uint8_t i = 0;
	while (source[i] != '\0')
		{
		target[i+begin_idx] = source[i];
		i++;
		}

}

void str_from_uint8_hex(uint8_t input_value, char *output_str)
	{
	str_merge("0x", output_str, 0);
	s_return_hex(input_value>>4, output_str+2);
	s_return_hex(input_value, output_str+3);
	output_str[4] = '\0';
	}


void str_from_uint32_hex(uint32_t input_value, char *output_str)
	{
	str_merge("0x", output_str, 0);
	s_return_hex(input_value>>28, output_str+2);
	s_return_hex(input_value>>24, output_str+3);
	s_return_hex(input_value>>20, output_str+4);
	s_return_hex(input_value>>16, output_str+5);
	s_return_hex(input_value>>12, output_str+6);
	s_return_hex(input_value>>8, output_str+7);
	s_return_hex(input_value>>4, output_str+8);
	s_return_hex(input_value, output_str+9);
	output_str[10] = '\0';
	}


void str_from_uint8(uint8_t input_value, char *output_str)
	{
	/* Divider 100! 255 value which is max value of
	 * uint8. */
	return_string_from_uint((uint32_t)input_value, output_str, 100, 0);
	}

void str_from_uint32(uint32_t input_value, char *output_str)
	{
	/* Divider 1000000000! 4,294,967,295 value which is max value of
	 * uint32. */
	return_string_from_uint(input_value, output_str, 1000000000, 0);
	}

void str_from_int32(int32_t input_value, char *output_str)
	{
	uint8_t str_idx = 0;

	/* If value is negative */
	if (input_value < 0)
		{
		output_str[str_idx] = '-';
		input_value = input_value * -1;
		str_idx++;
		}
	
	/* Now value is "positive", so we can use same algorythm as to uint type. */
	return_string_from_uint((uint32_t)input_value, output_str, 1000000000, str_idx);
	}

extern errorc_t str_compare( char *string_one, char *string_two, uint8_t max_idx_to_compare)
	{
	int32_t string_idx = -1;

	do {
		string_idx++;
		if(	string_one[string_idx] != string_two[string_idx] )
			{
			return EC_FALSE;
			}
	} while ( string_one[string_idx] != '\0' && string_idx < max_idx_to_compare);

	return EC_TRUE;
	}