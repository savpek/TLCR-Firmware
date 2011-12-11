/*
 * terminalapi.c
 *
 * Created: 10.12.2011 13:49:29
 *  Author: savpek
 */ 

#include "compiler.h"
#include "./api/terminalapi/inc/terminalapi.h"
#include "./api/terminalapi/config/terminalapi_config.h"

/* Init terminal api, this must be called before any terminal api functions are used! */
void terminalapi_init()
	{
	#ifndef TERMINALAPI_PRINT_FUNCTION ||\
			TERMINALAPI_PUTCHAR_FUNCTION ||\
			TERMINALAPI_GETCHAR_FUNCTION ||\
			TERMINALAPI_READCHAR_FUNCTION
	# error "TERMINALAPI output functions not defined!"
	#endif
	
	terminalapi_print = TERMINALAPI_PRINT_FUNCTION;
	terminalapi_put_char = TERMINALAPI_PUTCHAR_FUNCTION;
	terminalapi_get_char = TERMINALAPI_GETCHAR_FUNCTION;
	terminalapi_read_char = TERMINALAPI_READCHAR_FUNCTION;
	
	TERMINALAPI_INIT_OUTPUT_DEVICE();
	}

/* This function read command from defined stream. Stays in until
 * user sends NL character. */
void terminalapi_get_command(terminalapi_cmd_t *cmd_struct)
	{
	char char_temp = 0;
	uint8_t current_string_idx = 0;
		
	/* Check for newline */
	while (	(char_temp = terminalapi_get_char()) != '\r')
		{	
		/* If character is backspace. Backspace can be 0x08 (BS) or DEL (0x7F),
		 * Or probably any other character, thanks for standards ... */
		if(	(	char_temp == 0x08 ||
				char_temp == 0x7F	) &&
				current_string_idx > 0)
				{
				cmd_struct->command_str[current_string_idx] = '\0';
				current_string_idx--;
				
				/* Echo character */
				terminalapi_put_char(char_temp);
				
				continue;
				}
			
		/* Checks that string is inside of size constraint. If not
		* don't add any characters before you have removed some. */
		if(TERMINALAPI_CMD_LENGTH-1 <= current_string_idx) continue;	
			
		/* If character is acceptable (normal lowercase character, number or (-) mark */
		switch (char_temp)
			{
			case 'a' ... 'z':
			case '0' ... '9':
			case '-':
			case '_':
			case ' ':
				cmd_struct->command_str[current_string_idx] = char_temp;
				
				/* Echo */
				terminalapi_put_char(char_temp);
				
				current_string_idx++;
			default:
				break;
			}
		}
	
	/* At this point we are at end of command string */
	cmd_struct->command_str[current_string_idx] = '\0';
	
	/* Set last markable character from string as end of value. */
	cmd_struct->value_end_idx = current_string_idx-1;
	
	/* Next we will lookup for begin and end points of certain parts of command. */
	/* Find end of command part */
	current_string_idx = 0;
	while(	cmd_struct->command_str[current_string_idx] != '\0' &&
			cmd_struct->command_str[current_string_idx] != ' ')
			{
			current_string_idx++;	
			}
	
	/* First '\0' or ' ' counts as end of command */
	cmd_struct->cmd_end_idx = current_string_idx-1;
	
	/* Find value begin idx. */
	current_string_idx++;
	while(  cmd_struct->command_str[current_string_idx] == ' ' )
			{
			current_string_idx++;
			}
	cmd_struct->value_begin_idx = current_string_idx;
	
	/* Lets check that is there value string given. 
	 * Mark flag if it is.*/
	if(cmd_struct->cmd_end_idx != cmd_struct->value_end_idx)
		{
		cmd_struct->has_value_flag = EC_TRUE;	
		}
	}

extern errorc_t terminalapi_try_get_int32(terminalapi_cmd_t *cmd_struct, int32_t *ret_int_value)
	{
	uint8_t string_current_idx = cmd_struct->value_end_idx;
	uint32_t multiplier = 1;

	/* This must be done! Oterwise old (random) 
	 * value will be added to new value */
	*ret_int_value = 0;

	/* Now we have begin point of value in value_begin_index and
	 * end point in string_current_index. Lets calculate value of
	 * string part. */
	do
		{
		/* If number is negative, invert value and end here. */
		if(	cmd_struct->command_str[cmd_struct->value_begin_idx] == '-' && 
			string_current_idx == cmd_struct->value_begin_idx )
			{
			*ret_int_value *= -1;
			break;
			}
		
		/* Changes ASCII mark to value. Made by reduce 48 from
		 * ASCII value (48 is point where 0 is in ASCII table).*/

		/* Checks that ASCII mark is in really number */
		if ((cmd_struct->command_str[string_current_idx] < '0')
				|| cmd_struct->command_str[string_current_idx] > '9')
			{
			terminalapi_print("Invalid value, number required!");
			return EC_FAILURE;
			}

		/* Adds value to return value, every run value gets 10x bigger */
		*ret_int_value += ( cmd_struct->command_str[string_current_idx] - '0' ) * multiplier;
		multiplier = multiplier * 10;

		/* Go to next character (number) from right to left. Stop if face beginning
		 * of the string, or '-' mark. */
		string_current_idx--;
		} while ( string_current_idx != cmd_struct->value_begin_idx-1 );
	
	return EC_SUCCESS;	
	}

/* This function requires that there are value string given.
 * Return:  EC_FALSE if no value string available. 
			EC_TRUE if is. 
 * Also prints error message to output stream. */
extern errorc_t terminalapi_check_value(terminalapi_cmd_t *cmd_struct)
	{
	if(EC_TRUE != cmd_struct->has_value_flag)
		{
		terminalapi_print("You didn't give value for command!");
		return EC_FALSE;
		}
	else
		{
		return EC_TRUE;
		}
	}

/* This function tries run program (which are defined in terminalapi_program_list.h and .c).
 * Returns: EC_FAILURE if program doesn't excist.
 *			EC_SUCCESS if program is runned correctly. */
extern errorc_t terminalapi_try_run_program(terminalapi_cmd_t *cmd_struct, terminalapi_program_t program_list[])
	{
	uint32_t program_idx = 0;
	
	/* Loop through command list and try find correct program to run. */
	while(program_list[program_idx].program_pointer != 0)
		{
		/* Command string can also end empty space (after space comes value), for that reason third argument
		 * is set to ' '. */
		if( chrstr_string_compare(cmd_struct->command_str, program_list[program_idx].command_string, cmd_struct->cmd_end_idx) )
			{
			/* If strings match, run program in terminal */
			(*(*program_list[program_idx].program_pointer))(cmd_struct);
			return EC_SUCCESS;
			}
		program_idx++;
		}
	
	return EC_FAILURE;
	}

/* Ask "are you sure" question in terminal. 
 * Returns: EC_TRUE, EC_FALSE */
extern errorc_t terminalapi_ask_sure( )
	{
	terminalapi_print("Are you sure?");

	if( 'y' == terminalapi_get_char())	
		{
		return EC_TRUE;
		}
	else
		{
		return EC_FALSE;
		}
	}

/* Ask "do you want to continue" in terminal. */
extern errorc_t terminalapi_ask_continue( )
	{
	terminalapi_print("Continue, are you sure?\r\n");
	
	if( 'y' == terminalapi_get_char())	
		{
		return EC_TRUE;	
		}
	else
		{
		return EC_FALSE;
		}	
	}