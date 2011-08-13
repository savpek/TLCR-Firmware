/*
 * terminal_subp.c
 *
 * Created on: 27.7.2011
 *     Author:	Savpek Electronics
 *     Mail:	savpekelectronics@gmail.com
 *  
 * terminal_subp.c:
 *	Includes sub-programs for terminal. User can run these
 *	sub-programs from terminal. Programs to change settings, upload
 *	data to flash, print device status, manual drive and so on...
 */

#include "compiler.h"
#include "subprog.h"
#include "terminal.h"
#include "error_codes.h"
#include "storage.h"
#include "ascii_chars.h"

static errorc_t _is_same_or_same_type(char c_this, char c_to)
	{
	if(c_this == c_to) return EC_TRUE;
	
	/* Check that are both number values */
	if(c_this >= 48 && c_this <= 58 
		&& c_to >= 48 && c_to <= 58) return EC_TRUE;
		
	return EC_FALSE;
	}

/*@ Saves streaming data to terminal memory. See data file syntax from
 * flash.txt (root directory of project). */
static errorc_t _try_store(uint8_t segment_id, volatile uint16_t *byte_id, uint8_t char_temp)
	{
	const char  list_begin[6] = "##00##";
	const char  list_end[6] = "######";
	static char check_buffer[6] = {0};
	static uint8_t blist_index = 0;
	static uint8_t elist_index = 0;
	uint8_t write_counter = 0;
	
	/* Basic idea is that there are three states:
	 * WRITE: In this state, function writes sended
	 *  characters (and hole buffer) to memory.
	 * PAUSE: In this state, function simply waits
	 *  ##00## string. Ignores all else.
	 * WAIT: In this states, function have received part
	 *	of begin or end string. Now it waits rest of those string
	 *  received, or if not, write collected (string which includes # 
	 *  marks but isn't end mark) buffer and continue
	 *  normally. */
	static enum write_mode_t {
		WRITE,
		PAUSE,
		WAIT
		} write_flag = PAUSE;
	
	/* If esc is pressed, quit! */
	if(char_temp == ASCII_ESC) return EC_EXIT;
	
	/* Put received string in to the buffer, in cases
	 * when there haven't been # marks before this letter and
	 * this character isn't #, buffer
	 * should be empty (and stay empty as this character
	 * is written immediatly to the memory). */ 
	check_buffer[blist_index] = char_temp;
	
	/* Here we check that character isn't part of markabe strings
	 * (isn't begin or end marking of segment) */	
	if(_is_same_or_same_type(list_begin[blist_index], char_temp))
		{
		blist_index++;
		if(write_flag != PAUSE) write_flag = WAIT;
		}
	else if(write_flag != PAUSE) write_flag = WRITE;		
	
	if(_is_same_or_same_type(list_end[elist_index], char_temp))
		{
		elist_index++;
		if(write_flag != PAUSE) write_flag = WAIT;
		}
	else if(write_flag != PAUSE) write_flag = WRITE;

	/* If buffer is full, that means that either it marks end or beginning
	 * of segment. Next means that string have included end markings. */
	if(elist_index == 6)
		{
		write_flag = PAUSE;
		elist_index = 0;
		blist_index = 0;
		return EC_END;
		}

	/* or begin ##00## */
	if(blist_index == 6)
		{
		write_flag = WRITE;
		blist_index = 0;
		elist_index = 0;
		return EC_GOTO_NEXT;
		}
	
	/* We use blist_index in loops, so if blist is smaller than elist
	 * which means that we are maybe reaching end mark, make blist as
	 * big. */
	if(elist_index>blist_index) blist_index = elist_index;
	
	/* If we have permission to write, lets write entire buffer. (which may
	 * include part of end/begin mark, or just maybe one letter just received. */
	while(write_flag == WRITE &&
			blist_index >= write_counter)
			{
			usart_putchar(TERMINAL_USART, check_buffer[write_counter]);			
			storage_write_segment(segment_id, *byte_id, check_buffer[write_counter]);
			(*byte_id)++;
			
			if(check_buffer[write_counter] == ASCII_CR)
				{
				usart_putchar(TERMINAL_USART, ASCII_NL);
				storage_write_segment(segment_id, *byte_id, ASCII_NL);
				(*byte_id)++;
				}
				
			write_counter++;
			}
		
	/* If we have written stuff, lets reset buffer indexes */
	if(write_counter >= blist_index)
		{
		blist_index = 0;
		elist_index = 0;	
		}		
	
	return EC_DONE;
	}



/*@ With this terminal program, you can upload certain text file
 *	(see example flash.txt from root of project) to flash storage. */
extern void subprog_upload_flash_content(char* params)
	{
	int char_temp = 0;
	uint8_t segment_id = 0;
	volatile uint16_t byte_id = 0;
	uint16_t no_succes_count = 0;
	errorc_t return_value = 0;
	
	/* Ask if user really want rewrite flash content. This is for 
	 * make sure that user don't accidently erase whole content */
	usart_write_line(TERMINAL_USART, "Rewrite entire flash: ");
	
	if(!subprogc_ask_are_sure()) return;
	
	usart_write_line(TERMINAL_USART, "\r\nErasing chip...!\r\n");
	if(!storage_erase_all())
		{
		usart_write_line(TERMINAL_USART, "\r\n...Flash erase failed!\r\n");	
		return;	
		}
	
	usart_write_line(TERMINAL_USART, "\r\n...Flash erase complete!\r\n");
	
	usart_write_line(TERMINAL_USART, "Send file and press ESC: \r\n");
	
	while(1)
		{
		/* In here, you have to control flow since flash cannot write
		 * data as fast as usart can send it. */
		usart_putchar(TERMINAL_USART, ASCII_XON);
		usart_putchar(TERMINAL_USART, ASCII_XOFF);
		
		/* This strange loop thing because after you have send
		 * XON and XOFF, you may receive random number of characters.
		 * In addition you cannot stop compeletly to wait characters
		 * because then you are stuck forever since state have been set
		 * XOFF. As this isn't enough, character may not be received as
		 * fast as loop might go, so you have to try series of times before
		 * you can be sure that there will be no more chars coming. */
		while(no_succes_count++ < 500)
			{
			/* If there are stuff in buffer, examine it */
			if(usart_read_char(TERMINAL_USART, &char_temp) == USART_SUCCESS)
				{
				/* Try save data, this function saves data in memory and
				 * parses begin and end marks of segments from datafile. */
				return_value = _try_store(segment_id, &byte_id, char_temp);
				
				/* If ESC is pressed, quit program! */
				if (return_value == EC_EXIT) return;
				/* If returns that ##00## mark is found, change to next segment */
				else if(return_value == EC_GOTO_NEXT)
					{
					usart_write_line(TERMINAL_USART, "## SEGMENT ");
					usart_write_hex(TERMINAL_USART, segment_id, 8);
					usart_write_line(TERMINAL_USART, " BEGIN ##");
					}
				/* If end mark is found. */
				else if (return_value == EC_END)
					{
					usart_write_line(TERMINAL_USART, "## SEGMENT ENDS ##\r\n\n");
					segment_id++;
					byte_id = 0;
					}
				}
			}
			
		no_succes_count = 0;
		
		/* This is here because just for sure, if you get an error
		 * usart halts working compeletly otherwise. */
		usart_reset_status(TERMINAL_USART);
		}
		
	subprogc_xon();
	}

/*@ Prints selected segment data to terminal in format
 * 0xXX A, so first hex and then char presentation of same
 * segment. */	
extern void subprog_flash_print_segment(uint32_t params)
	{
	uint8_t segment_id = (uint8_t) params;
	usart_write_line(TERMINAL_USART, "\r\nSegment data:\r\n");
	uint16_t next_byte = 0;
	uint8_t print_char;
	
	while(storage_read_segment(segment_id, next_byte, &print_char) == EC_DONE)
		{
		usart_write_hex(TERMINAL_USART, print_char, PRINT_BYTE);
		usart_putchar(TERMINAL_USART, ASCII_SPACE);
		
		/* Print character must be in printable ASCII range */
		if (print_char >= 32 && print_char < 127)
			{
			usart_putchar(TERMINAL_USART, print_char);		
			}
		else 
			{
			usart_putchar(TERMINAL_USART, ASCII_SPACE);
			}
		
		usart_putchar(TERMINAL_USART, ASCII_SPACE);
		usart_putchar(TERMINAL_USART, ASCII_SPACE);
		usart_putchar(TERMINAL_USART, ASCII_SPACE);
		
		if (next_byte%20 == 0)
			{
			usart_write_line(TERMINAL_USART, "\r\n");	
			}
		
		/* Because segments are so large, its good to have
		 * change to quit before end. */
		if (next_byte%500 == 0 && next_byte != 0)
			{
			if(subprogc_ask_continue() != EC_TRUE)
				{
				break;	
				}
			}
		
		next_byte++;
		}	
	}

/*@ This subprogram simply checks storage flash chips ID.
 * This is to quick check that chip is responding correctly. */	
extern void subprog_flash_check(char *params) 
	{
	uint16_t flash_id;
	
	flash_id = storage_get_flash_id();
	
	if(flash_id == 0)
		{
		usart_write_line(TERMINAL_USART, "Fail! Memory didn't return ID.\r\n");	
		}
	else
		{
		usart_write_line(TERMINAL_USART, "Pass! Memory ID ok.\r\n");
		}		
			
	}

/*@ Writes help file to terminal. Gets defined segment from flash
 *	where helpfile should be written and prints it to terminal. */
extern void subprog_flash_print_help_file(char* params)
	{
	usart_write_from_storage(TERMINAL_USART, STORAGE_HELPFILE_ID);
	}		

/*@ Counts selected segment data as text and prints it to terminal. */
extern void subprog_flash_print_segment_text(uint32_t segment_id)
	{
	usart_write_from_storage(TERMINAL_USART, segment_id);
	}
	
/*@ Writes 'c' letter at addr 15 in selected segment. Meant to be
 * simply function that proves in problem situations that flash is writing
 * correctly. */	
extern void subprog_flash_test_write(uint32_t segment_id)
	{
	storage_write_segment(segment_id, 15, 'c');
	usart_write_line(TERMINAL_USART, "Written A in addr 15!");
	}
	
/*@ Erases hole chip at once, this is for test that erase is working */
extern void subprog_flash_test_erase(char *params)
	{
	storage_erase_all();	
	usart_write_line(TERMINAL_USART, "Flash erased ...!");
	}