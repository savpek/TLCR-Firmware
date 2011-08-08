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
#include "terminal_subp.h"
#include "spi.h"
#include "error_codes.h"

/* Definations to make code more easy to read */
#define TERMINAL_THREAD_TIMEOUT_INFINITE 77
#define TERMINAL_COMMAND_MAX_LENGTH 30

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
static uint32_t _get_command_value_int(char* command_string)
	{
	int value_begin_index = 0;
	int string_current_index = 0;
	int return_value = 0;
	int multiplier = 1;

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
			return EC_OUT_OF_RANGE;
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
			return EC_NOT_NUMBER;
			}

		/* Adds value to return value, every run value gets 10x bigger */
		return_value += (command_string[string_current_index]-48)*multiplier;
		multiplier = multiplier*10;

		/* Go to next character (number) from right to left */
		string_current_index--;
		}

	return return_value;
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

/*@ Thread for terminal. This parses terminal commands and drive sub-programs
 * under terminal when needed. */
extern void terminal_thread(void* parameters)
	{
	char command_str[TERMINAL_COMMAND_MAX_LENGTH];
	uint32_t command_value = 0;
	while(1)
		{
		terminal_write_line_const(" TL>")
		_get_command (command_str);
		
		if(_is_command_same(command_str, "flash_print_segment"))
			{
			command_value = _get_command_value_int(command_str);
			subprogram_flash_print_segment(command_value);
			}
		else if(_is_command_same(command_str, "flash_check"))
			{
			subprogram_flash_check();
			}
		else
			{
			terminal_write_line_const("Command not found.\n\r");	
			}
		
		terminal_write_line_const("\r\n");
		}
	}

/* @Inits serial transmission
 * from MCU. Uses USART driver from Atmels framework.*/
extern void terminal_init()
	{
	/* Settings for usart initialization. Atmels USART-driver
	 * reguires settings in usart_options_t type struct.	*/
	static const usart_options_t USART_OPTIONS =
	   {
	     .baudrate     = TERMINAL_BAUDRATE,
	     .charlength   = TERMINAL_CHARLENGTH,
	     .paritytype   = TERMINAL_PARITYPE,
	     .stopbits     = TERMINAL_STOPBITS,
	     .channelmode  = TERMINAL_CHANNELMODE
	   };

	/* Map ports for USART, Atmel driver function reguires mapping
	 * in gpio_map_t format. */
	static const gpio_map_t USART_GPIO_MAP =
	   {
	     {TERMINAL_USART_RX_PIN,TERMINAL_USART_RX_FUNCTION},
	     {TERMINAL_USART_TX_PIN, TERMINAL_USART_TX_FUNCTION}
	   };

	/* Assign GPIO to USART. */
	gpio_enable_module(USART_GPIO_MAP,
	                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

	/* Initialize USART in RS232 mode. */
	usart_init_rs232(TERMINAL_USART, &USART_OPTIONS, MCU_PBA_F);
	}

/*@ Writes non constant string to terminal */
extern void terminal_write_line(char* line_string)
	{
	int i = 0;
	while (line_string[i] != 0)
		{
		usart_putchar(TERMINAL_USART, line_string[i]);
		i++;	
		}
	}


/*@Degug message function, prints variable as bit value to terminal */
extern void terminal_print_bits (int32_t bit_value, uint8_t print_length)
	{
	int8_t bit_shift = print_length;
	
	while (bit_shift >= 0)
		{
		/* (bit_value>>bit_shift)&0x0001 gives 0 or 1 as result debending shifted
		 * bit. By adding 48 we move it to number area in ASCII table. */
		usart_putchar(TERMINAL_USART, (((bit_value>>bit_shift)&0x0001)+48));	
		
		/* Print space by every 4 bits, makes reading much easier. */
		if (bit_shift%4 == 0)
			{
			usart_putchar(TERMINAL_USART,  TERMINAL_SPACEBAR);
			}
		
		bit_shift--;
		}		
	}

/*@Degug message function, prints variable as hex value to terminal */
extern void terminal_print_hex (int32_t hex_value, uint8_t print_length)
	{
	int32_t hex_value_temp = 0;
	int8_t bit_shift = print_length-4;
	
	usart_write_line(TERMINAL_USART, "0x");
	
	while(bit_shift >= 0)
		{
		/* First shift bits (from 12->0, 4 bit steps). In this order because 
		 * most markable bits must be printed first to terminal. */
		hex_value_temp = hex_value>>bit_shift;
		
		/* After shift, 4 last binary numbers are one character,
		 * which should get printed. So we mask rest than last 4 markable
		 * bits out. */
		hex_value_temp = hex_value_temp&0x000F;
		
		/* If character is bigger than 9 (A-F letter) add number 65
		 * to move it correct point in ASCII table. (point where A letter is). */
		if(hex_value_temp > 9)
			{
			hex_value_temp += 65-10;	
			}
		/* If less than 9, add number 48 to print it as number from ASCII table */
		else
			{
			hex_value_temp += 48;	
			}
		
		usart_putchar(TERMINAL_USART, hex_value_temp);
		
		bit_shift -= 4;
		}
	}

/* @XON/XOF protocol functions. */
extern void terminal_xon() 
	{
	/* Send XON (RESUME) character to terminal */
	usart_putchar(TERMINAL_USART, 0x11);
	}
	
extern void terminal_xoff()
	{
	/* Send XOFF (PAUSE) character to terminal */
	usart_putchar(TERMINAL_USART, 0x13);		
	}