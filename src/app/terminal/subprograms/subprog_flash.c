/*
 * subprog_flash.c
 *
 * Created: 5.12.2011 16:26:13
 *  Author: savpek
 */ 

#include "compiler.h"

#include "./app/terminal/inc/terminal.h"

/* TLCR libs */
#include "./api/errorh/inc/errorh.h"
#include "./api/flashmem/inc/flashmem.h"
#include "./app/terminal/common/common_functions/inc/terminal_common_functions.h"
#include "./api/chrstr/inc/chrstr.h"

/* Functions for subprog_flash_dump */
#define SBPROG_FLASH_ROW_LENGTH 16
static void s_print_hex_row(uint32_t flash_addr)
	{
	uint8_t item_count = 0;
	uint8_t byte_temp = 0;
	uint8_t hex_str_temp[8] = {0};
	
	/* In beginning of row, print addr: */
	chrstr_uint32_to_hex_str(flash_addr, hex_str_temp);
	usart_write_line(hex_str_temp);
	usart_write_line(":");
	
	/* Print 16 cells off data */
	while ( item_count < 16 )
		{
		byte_temp = flashmem_read_uint8_t(flash_addr+item_count);
		
		chrstr_uint8_to_hex_str(byte_temp, hex_str_temp);
		
		usart_write_line(" ");
		usart_write_line(hex_str_temp);
		usart_write_line(" ");
		
		item_count++;
		}
		
	/* End of row */
	usart_write_line("\r\n");
	}

/* This subprogram begins dump flash memory content to terminal
 * screen. */
void subprog_flash_dump(char* arg_str)
	{
	uint32_t flash_addr = 0;
	uint32_t row_count = 0;
	
	/* Until we meet end of flash or user select not to continue,
	 * we print data. */
	do
		{
		/* Print 30 rows of data */
		while(row_count <= 30 && flash_addr+16 < FLASHMEM_SIZE)
			{
			s_print_hex_row(flash_addr);
			flash_addr += 16;
			row_count++;
			}
		row_count = 0;
		
		/* Ask that do you want to continue. */	
		usart_write_line("\n\rContinue?\n\r");
		} while(terminal_ask_are_sure() == EC_TRUE && flash_addr+16 < FLASHMEM_SIZE);
	
	usart_write_line("Exit...");
	}