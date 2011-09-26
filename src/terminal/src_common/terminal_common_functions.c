/*
 * subprog_common.c
 *
 * Created: 8.8.2011 20:33:21
 *  Author: savpek
 */ 

#include "compiler.h"
#include "subprograms.h"
#include "error_codes.h"
#include "usart.h"
#include "storage.h"
#include "ascii_chars.h"

/*@ -->Constant printing version of this is simply macro that points data to
 *  usart_write_line() function. See more from subprog.h*/

/* @XON/XOF protocol functions. */
extern void subprogc_xon(void) 
	{
	/* Send XON (RESUME) character to terminal */
	 usart_putchar(TERMINAL_USART, 0x11);
	}
	
extern void subprogc_xoff(void)
	{
	/* Send XOFF (PAUSE) character to terminal */
	 usart_putchar(TERMINAL_USART, 0x13);		
	}


/*@This function sends "Are you sure?" dialog through terminal */
//NOTTESTED
extern errorc_t subprogc_ask_are_sure(void)
	{
	char char_temp = 0;
	usart_write_line(TERMINAL_USART, "\r\nAre you sure? (y/n): ");
	char_temp = usart_getchar(TERMINAL_USART);
	usart_putchar(TERMINAL_USART, char_temp);
	usart_write_line(TERMINAL_USART, "\r\n");

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
extern errorc_t subprogc_ask_continue(void)
	{
	char char_temp = 0;
	usart_write_line(TERMINAL_USART, "\r\nContinue? (y/n): ");
	char_temp = usart_getchar(TERMINAL_USART);
	 usart_putchar(TERMINAL_USART, char_temp);
	usart_write_line(TERMINAL_USART, "\r\n");
	
	if (char_temp == 'y')
		{
		return EC_TRUE;	
		}
	else
		{
		return EC_FALSE;	
		}
	}