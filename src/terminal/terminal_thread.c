/*
 * serterminal.c
 *
 * Created on: 27.7.2011
 *     Author:	Savpek Electronics
 *     Mail:	savpekelectronics@gmail.com
 *  
 * terminal.c:
 *	Library for creating terminal over USART.
 */

#include "terminal.h"
#include "subprograms.h"
#include "error_codes.h"
#include "usart.h"
#include "ascii_chars.h"
#include "terminal_program_list.h"

/* How long command writed in terminal can be (including parameters).*/
#define TERMINAL_COMMAND_MAX_LENGTH 30

/*!! Private static functions for terminal thread own use */
/* @Function reads (and waits) command that ends with newline and
 * returns it to string. */
static void _get_command (char* ret_command_string)
	{
	char received_char = 0;
	int string_current_index = 0;

	/* Loops, reads transmission to string until ended by CR */
	while(1)
		{
		received_char = usart_getchar(TERMINAL_USART);

		/* Don't accept special letters or other ASCII standard
		 * trash. */
		if (((received_char <= 123 && received_char >= 48)
				|| received_char == TERMINAL_SPACEBAR)
				&& string_current_index < TERMINAL_COMMAND_MAX_LENGTH)
			{
			ret_command_string[string_current_index] = received_char;

			usart_putchar(TERMINAL_USART, received_char);

			string_current_index++;
			}
		/* If character is backspace. */
		else if (received_char == 0x7F && string_current_index > 0)
			{
			string_current_index--;
			usart_putchar(TERMINAL_USART, received_char);
			}
		/* If enter (0x0D)*/
		else if (received_char == 0x0D)
			{
			usart_putchar(TERMINAL_USART, received_char);
			usart_putchar(TERMINAL_USART, '\n');

			/* Null character to mark end of line. */
			ret_command_string[string_current_index] = 0;

			/* Command string is completed. */
			break;
			}
		}
	}

/*@ This function is used to separate value of command from 
 * command string. Returns numerical value as int. If value
 * in command is not integer, returns SERTERMINAL_VALUE_ERROR. */
static errorc_t _try_get_value_int(char* command_string, uint32_t *return_value)
	{
	uint8_t value_begin_index = 0;
	uint8_t string_current_index = 0;
	uint8_t multiplier = 1;

	/* First need to find point where value begins (first space) */
	while (1)
		{
		/* Checks SPACE */
		if(command_string[string_current_index] == TERMINAL_SPACEBAR)
			{
			value_begin_index = string_current_index;
			break;
			}
		/* If no value given (gets end of string or
		 * get out of maximum length of string */
		else if ((command_string[string_current_index] == 0)
				|| (string_current_index == TERMINAL_COMMAND_MAX_LENGTH))
			{
			return EC_FAIL;
			}
		string_current_index++;
		}

	/* If value begin point find succesfully, next find end of command
	 * string. */
	while ((string_current_index < TERMINAL_COMMAND_MAX_LENGTH)
			&& (command_string[string_current_index] != 0))
		{
		string_current_index++;
		}

	/* Reduce by one to get pointer from 0 to last markable character */
	string_current_index--;

	/* Now we have begin point of value in value_begin_index and
	 * end point in string_current_index. Lets calculate value of
	 * string part. */
	while (string_current_index > value_begin_index)
		{
		/* Changes ASCII mark to value. Made by reduce 48 from
		 * ASCII value (48 is point where 0 is in ASCII table).*/

		/* Checks that ASCII mark is in really number */
		if ((command_string[string_current_index]-48 < 0)
				|| command_string[string_current_index]-48 > 9)
			{
			return EC_FAIL;
			}

		/* Adds value to return value, every run value gets 10x bigger */
		*return_value += (command_string[string_current_index]-48)*multiplier;
		multiplier = multiplier*10;

		/* Go to next character (number) from right to left */
		string_current_index--;
		}

	return EC_DONE;
	}
	 
/*@ This function returns point where value string begins.
 * Example: "set_direction left", this function returns value 14. */
static uint16_t _get_command_value_begin_index(char* command_string)
	{
	int string_current_index = 0;

	/* First need to find point where value begins (first space) */
	while (1)
		{
		/* Checks SPACE */
		if(command_string[string_current_index] == TERMINAL_SPACEBAR)
			{
			break;
			}
		/* If no value given (gets end of string or
		 * get out of maximum length of string */
		else if ((command_string[string_current_index] == 0)
				|| (string_current_index == TERMINAL_COMMAND_MAX_LENGTH))
			{
			return EC_OUT_OF_RANGE;
			}
		string_current_index++;
		}
		
	return string_current_index+1;
	}

/*@ Compares two strings, ended by NULL (or SPACE: end of command
 * part of string from terminal). If commands are equal and command is not 
 * empty, returns true. */
static errorc_t _is_command_same (char* command, const char* compare_to_str)
	{
	uint8_t string_index = 0;
	uint8_t compare_counter = 0;
	
	/* Checks equality, if reaches end (SPACE or NULL) per. character
	 * in same time counts length of command (length must be compared so that
	 * set is not equal with set_speed! */
	while(command[string_index] != 0 //Null
			&& command[string_index] != TERMINAL_SPACEBAR) //Space
		{
		/* Every character of command must be identical */
		if(command[string_index] != compare_to_str[string_index])
			{
			return EC_FALSE;
			}
		string_index++;
		}
	
	/* Counts length of compare string */	
	while(compare_to_str[compare_counter] != 0) compare_counter++;	
	
	/* Length of commands must be identical */
	if(compare_counter != string_index)
		{
		return EC_FALSE;
		}			
	
	/* If reaches this point, means strings are equal */
	return EC_TRUE;
	}

/*@ This function try run program with selected name */
static errorc_t _try_command (char* command_str)
	{
	uint8_t arindex = 0;
	uint32_t param_uint32 = 0;

	/* We loop through program array and try find match */
	while(_subprog_array[arindex].command_str[0] != 0)
		{
		if(_is_command_same(command_str, _subprog_array[arindex].command_str))
			{
			/* When we find matching function. Lets see if given value is correct
			 * type for it. */
			if(_try_get_value_int(command_str, &param_uint32)
				&& _subprog_array[arindex].pointer_uint32 != 0)
				{
				/* Run program with parameter type of uint32 */
				(*_subprog_array[arindex].pointer_uint32)(param_uint32);
				return EC_DONE;
				}
			else if(_subprog_array[arindex].pointer_str != 0)
				{
				/* Run program with parameter type of string */
				(*_subprog_array[arindex].pointer_str)(command_str+_get_command_value_begin_index(command_str));
				return EC_DONE;
				}
			else
				{
				/* Didn't find correct kind of program. This happens when
				 * there is function for this command, but given value is 
				 * incorrect type. IE: set_direction 33 (param should be string,
				 * left or right) */
				return EC_VALUE_ERROR;	
				}
			}
		arindex++;
		}
		
	/* If function gets to this point, there is no correctly
	 * named subprogram in list. So command is invalid. */
	return EC_FAIL;	
	}

/*@ Thread for terminal. This parses terminal commands and drive sub-programs
 * under terminal when needed. */
extern void terminal_thread(void* params)
	{
	/* Write terminal header data :) */
	usart_write_line(TERMINAL_USART, "\r\n\nTimelapse terminal\r\n");
	usart_write_line(TERMINAL_USART, "Author: Savpek Electronics 2011\r\n");
	
	char command_str[TERMINAL_COMMAND_MAX_LENGTH];
	
	/* Save return value of _try_command */
	char try_command_ret = 0;
	
	/* Infinite thread loop */
	while(1)
		{
		/* Send XON to make sure terminal is not in pause state. */
		usart_putchar(TERMINAL_USART, ASCII_XON);
		
		/* Allso reset usart status bit for sure, this because if usart
		 * have faced error (overrun for most likely) it will be halted
		 * until this bit is reseted. */
		usart_reset_status(TERMINAL_USART);	

		/* One loop is one command run, so print terminal mark */
		usart_write_line(TERMINAL_USART, " TL>");
		
		/* Wait until get command (string ended with enter press) */
		_get_command (command_str);
		
		/* Try run command  just received. See stucture of commands
		 * from begining of this file. */
		try_command_ret = _try_command(command_str);
	
		/* Then we check were command run correctly or was there
		 * error. */
		if (try_command_ret == EC_FAIL)
			{
			usart_write_line(TERMINAL_USART, "Invalid command!");	
			}
		else if (try_command_ret == EC_VALUE_ERROR)
			{
			usart_write_line(TERMINAL_USART, "Invalid value!");
			}
			
		/* Newlines are nice eh? */	
		usart_write_line(TERMINAL_USART, "\n\r");
		}		
	}