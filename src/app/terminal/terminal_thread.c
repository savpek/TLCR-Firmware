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

#include "compiler.h"
#include "./terminalapi/inc/terminalapi.h"
#include "./app/terminal/config/terminal_program_list.h"
#include "./config/FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

/*@ Thread for terminal. This parses terminal commands and drive sub-programs
 * under terminal when needed. */
void terminal_thread(void* arguments)
	{
	/* First init TERMINALAPI transmission functions */
	terminalapi_init();
	/* Write terminal header data :) */
	terminalapi_print("\r\n\nTLCR TERMINAL\r\n");
	terminalapi_cmd_t command_struct;
	/* Infinite thread loop. Never exit. */
	while(1)
		{
		/* One loop is one command run, so print terminal mark */
		terminalapi_print("TLCR>");
		
		/* Wait until get command (string ended with enter press). */
		terminalapi_get_command(&command_struct);
		
		terminalapi_print("\r\n");
		
		/* Try run program, basically tries find correct string from program list.
		 * If found match, runs program. */
		if (EC_FAILURE == terminalapi_try_run_program(&command_struct, &terminal_program_array))
			{
			terminalapi_print("Invalid command!\n\r");	
			}
		
		terminalapi_print("\r\n");
		}
	}