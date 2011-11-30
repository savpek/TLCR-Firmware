/*
 * subprog_common.c
 *
 * Created: 8.8.2011 20:33:21
 *  Author: savpek
 */ 

/* ASF */
#include "compiler.h"

/* Internal */
#include "./terminal/common/common_functions/inc/terminal_common_functions.h"

/* TLCR libs */
#include "./errorh/inc/errorh.h"
#include "./usart/inc/usart.h"

/*!! Functions that checks arguments given for subprogram.
 *   Usage example:
 *		uint32 arg_int_value = 0;	 
 *		if (terminal_try_get_int_value(arg_str, arg_int_value) == EC_FAILED) return;
 *	 If argument isn't number, subprogram then exit with error message */
errorc_t terminal_try_get_int_value (char* arg_str, uint32_t* return_arg_value)
	{
	uint8_t string_current_index = 0;
	uint32_t multiplier = 1;

	/* If value begin point find succesfully, next find end of command
	 * string. Possible argument example:
	 * 33, then string_current_index = 2.*/
	while (arg_str[string_current_index] != 0)
		{
		string_current_index++;
		}
		
	/* Now we have begin point of value in value_begin_index and
	 * end point in string_current_index. Lets calculate value of
	 * string part. */
	do
		{
		/* If index is 2, we reduce it by 1. Then we are
		 * in point of last markable character from argument
		 * string. We do this as long as we are in beginning
		 * of the string.*/
		string_current_index--;
		
		/* Changes ASCII mark to value. Made by reduce 48 from
		 * ASCII value (48 is point where 0 is in ASCII table).*/

		/* Checks that ASCII mark is in really number */
		if ((arg_str[string_current_index]-48 < 0)
				|| arg_str[string_current_index]-48 > 9)
			{
			usart_write_line("Invalid value, number required!");
			return EC_FAILURE;
			}

		/* Adds value to return value, every run value gets 10x bigger */
		*return_arg_value += (arg_str[string_current_index]-48)*multiplier;
		multiplier = multiplier*10;

		/* Go to next character (number) from right to left */
		} while (string_current_index != 0);

	return EC_SUCCESS;
	}

/* For functions where argument is reguired.
 * Usage example:
 *	if (terminal_require_arg_str(arg_str) != EC_SUCCES) return; 
 * Then if argument is empty, subprogram exit with error message. */
errorc_t terminal_require_arg_str (char* arg_str )
	{
	uint8_t string_counter = 0;
	
	/* This function as simply: counts string length
	 * and if it is bigger than 0, there is argument given. */
	while(arg_str[string_counter] != 0) string_counter++;
	
	/* In case argument string is empty */
	if ( string_counter == 0)
		{
		usart_write_line("Invalid value!");
		return EC_FAILURE;	
		}
	/* In case there are argument given */
	else
		{
		return EC_SUCCESS;	
		}
	}

/*@This function sends "Are you sure?" dialog through terminal */
//NOTTESTED
extern errorc_t terminal_ask_are_sure(void)
	{
	uint8_t char_temp = 0;
	usart_write_line("\r\nAre you sure? (y/n): ");
	char_temp = usart_getchar();
	usart_putchar(char_temp);
	usart_write_line("\r\n");

	if (char_temp == 'y')
		{
		return EC_TRUE;	
		}
	else
		{
		return EC_FALSE;	
		}
	
	} 

/*@As if terminal user want to continue. Returns boolean */
extern errorc_t terminal_ask_continue(void)
	{
	uint8_t char_temp = 0;
	usart_write_line("\r\nContinue? (y/n): ");
	char_temp = usart_getchar();
	 usart_putchar(char_temp);
	usart_write_line("\r\n");
	
	if (char_temp == 'y')
		{
		return EC_TRUE;	
		}
	else
		{
		return EC_FALSE;	
		}
	}