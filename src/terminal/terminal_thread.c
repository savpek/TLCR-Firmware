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
#include "./terminal/subprograms/config/list_of_subprograms.h"
#include "error_codes.h"
#include "./usart/inc/usart.h"
#include "./usart/config/usart_settings.h"
#include "delay.h"

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
		received_char = usart_getchar();
		
		/* Don't accept special letters or other ASCII standard
		 * trash. */
		if (((received_char <= 123 && received_char >= 48)
				|| received_char == USART_ASCII_SPACE)
				&& string_current_index < TERMINAL_COMMAND_MAX_LENGTH)
			{
			ret_command_string[string_current_index] = received_char;

			usart_putchar(received_char);

			string_current_index++;
			}
		/* If character is backspace. */
		else if (received_char == 0x7F && string_current_index > 0)
			{
			string_current_index--;
			usart_putchar(received_char);
			}
		/* If enter (0x0D)*/
		else if (received_char == 0x0D)
			{
			usart_putchar(received_char);
			usart_putchar('\n');

			/* Null character to mark end of line. */
			ret_command_string[string_current_index] = 0;

			/* Command string is completed. */
			break;
			}
		} /* While 1 */
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
		if(command_string[string_current_index] == USART_ASCII_SPACE)
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
			&& command[string_index] != USART_ASCII_SPACE) //Space
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

	/* We loop through program array and try find match */
	while(_subprog_array[arindex].command_str[0] != 0)
		{
		if(_is_command_same(command_str, _subprog_array[arindex].command_str))
			{
			/* Run subprogram, gives value string as only argument for
			 * subprogram. If program uses any parameters, you should check
			 * value as first thing in subprogram function */
			(*_subprog_array[arindex].pointer)(command_str+_get_command_value_begin_index(command_str));
			return EC_SUCCESS;
			}
		arindex++;
		}
		
	/* If function gets to this point, there is no correctly
	 * named subprogram in list. So command is invalid. */
	return EC_FAILURE;	
	}

/*@ Thread for terminal. This parses terminal commands and drive sub-programs
 * under terminal when needed. */
extern void terminal_thread(void* params)
	{
	/* Write terminal header data :) */
	usart_write_line("\r\n\nTLCR TERMINAL\r\n");
	
	char command_str[TERMINAL_COMMAND_MAX_LENGTH];
	
	/* Save return value of _try_command */
	errorc_t try_command_ret = 0;
	unsigned long counteri = 0;
	/* Infinite thread loop */
	uint8_t char_temp = 0;
	while(1)
		{
		/* One loop is one command run, so print terminal mark */
		usart_write_line("TLCR>");
		
		/* Wait until get command (string ended with enter press) */
		_get_command (command_str);
		
		/* Try run command  just received. See stucture of commands
		 * from begining of this file. */
		try_command_ret = _try_command(command_str);
	
		/* Then we check were command run correctly or was there
		 * error. */
		if (try_command_ret == EC_FAILURE)
			{
			usart_write_line("Invalid command!");	
			}
		else if (try_command_ret == EC_VALUE_ERROR)
			{
			usart_write_line("Invalid value!");
			}
			
		/* Newlines are nice eh? */	
		usart_write_line("\n\r");
		}		
	}