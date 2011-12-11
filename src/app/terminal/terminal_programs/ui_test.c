/*
 * ui_test.c
 *
 * Created: 2.10.2011 18:45:36
 *  Author: savpek
 */ 

#include "compiler.h"
#include "usart_wb.h"
#include "terminal.h"
#include "terminal_usart_settings.h"
#include "button_config.h"
#include "encoder_config.h"
#include "encoder.h"
#include "button.h"

void subprog_ui_test ( char* argument_str )
	{
	uint8_t counter;
	uint32_t encoder_temp[10] = {0};
	int rec_buffer = 0;
			
	while(rec_buffer != 0x1B)
		{
		usart_read_char(TERMINAL_USART, &rec_buffer);
		
		while(counter < ENCODER_COUNT)
			{
			if(encoder_temp[counter] != encoder_handle[counter].step_count)
				{
				usart_write_line(TERMINAL_USART, "ENCODER:");
				usart_write_decimal(TERMINAL_USART, encoder_handle[counter].step_count);	
				usart_write_line(TERMINAL_USART, "\r\n");
				encoder_temp[counter] = encoder_handle[counter].step_count;
				}
			counter++;
			}
	
		counter = 0;
		while(counter < BUTTON_COUNT)
			{
			if(button_handle[counter].button_pressed_flag != 0)
				{
				button_handle[counter].button_pressed_flag = 0;
				usart_write_line(TERMINAL_USART, "BUTTON_PRESSED\r\n");
				}
			counter++;
			}
		counter = 0;
		
		}
	}