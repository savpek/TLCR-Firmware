/*
 * uprint.h
 * USART PRINT
 * Created: 12.8.2011 14:43:48
 *  Author: savpek
 * 
 */ 


#ifndef UPRINT_H_
#define UPRINT_H_

#include "terminal.h"

/* Define storage ID's where data is. This data is defined, and saved
 * to memory from flash.txt over terminal flash_rewrite subprogram (flash_upload_content
 * function in from source). */
#define UPRINT_S_ID_HELPFILE 0
#define UPRINT_S_ID_AUTHORFILE 1
#define UPRINT_S_ID_PROGRAMHELPFILE 2

/* Prints data from defined storage segment. */
extern void uprint_from_storage(uint8_t segment_id);

/* Print numbers as string format to serial */
extern void uprint_hex (int32_t hex_value, uint8_t print_length);
extern void uprint_bits (int32_t bit_value, uint8_t print_length);

/* Common print function for usart output */
extern void uprint_line(char* line_string);
#define uprint_cline(str) usart_write_line(TERMINAL_USART, str)
#define uprint_char(c) usart_putchar(TERMINAL_USART, c)

#endif /* UPRINT_H_ */