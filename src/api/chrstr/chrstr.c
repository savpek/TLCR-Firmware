/*
 * chars_strings.c
 *
 * Created: 20.11.2011 13:06:46
 *  Author: savpek
 */ 

#include "compiler.h"

#include "./api/chrstr/inc/chrstr.h"

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

void chrstr_uint8_to_hex_str(uint8_t input_value, uint8_t *output_str)
	{
	s_return_hex(input_value>>4, output_str);
	s_return_hex(input_value, output_str+1);
	output_str[2] = '\0';
	}


void chrstr_uint32_to_hex_str(uint32_t input_value, uint8_t *output_str)
	{
	s_return_hex(input_value>>28, output_str);
	s_return_hex(input_value>>24, output_str+1);
	s_return_hex(input_value>>20, output_str+2);
	s_return_hex(input_value>>16, output_str+3);
	s_return_hex(input_value>>12, output_str+4);	
	s_return_hex(input_value>>8, output_str+5);
	s_return_hex(input_value>>4, output_str+6);
	s_return_hex(input_value, output_str+7);
	output_str[8] = '\0';
	}


void chrstr_uint8_to_dec_str(uint8_t input_value, uint8_t *output_str)
	{
	uint32_t divider = 100;
	uint32_t str_idx = 0;

	do 
		{	
		if(( input_value / divider ) != 0 ||
			str_idx > 0)
			{
			output_str[str_idx] = ( input_value / divider )+'0';
			str_idx++;
			}
			
		input_value = input_value%divider;
		divider = divider/10;
		
		} while ( divider != 0 );
	
	output_str[str_idx] = '\0';
	}	

void chrstr_uint32_to_dec_str(uint32_t input_value, uint8_t *output_str)
	{
	/* Maximum value (increment of 10) that can be in uint32_t */
	uint32_t divider = 1000000000;
	uint32_t str_idx = 0;

	do 
		{	
		if(( input_value / divider ) != 0 ||
			str_idx > 0)
			{
			output_str[str_idx] = ( input_value / divider )+'0';
			str_idx++;
			}
			
		input_value = input_value%divider;
		divider = divider/10;
		
		} while ( divider != 0 );
		
	output_str[str_idx] = '\0';
	}
	
void chrstr_int32_to_dec_str(int32_t input_value, uint8_t *output_str)
	{
	/* Maximum value (increment of 10) that can be in uint32_t */
	uint32_t divider = 1000000000;
	uint32_t str_idx = 0;
	
	/* If value is negative */
	if (input_value < 0)
		{
		output_str[str_idx] = '-';
		input_value = input_value*-1;
		}
	else
		{
		output_str[str_idx] = ' ';	
		}
	
	str_idx++;
	
	/* Return number to string */
	do 
		{	
		if(( input_value / divider ) != 0 ||
			str_idx > 1)
			{
			output_str[str_idx] = ( input_value / divider )+'0';
			str_idx++;
			}
			
		input_value = input_value%divider;
		divider = divider/10;
		
		} while ( divider != 0 );
		
	output_str[str_idx] = '\0';
	}

