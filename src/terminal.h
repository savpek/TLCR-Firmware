/*
 * terminal.h
 *
 * Created on: 27.7.2011
 *     Author:	Savpek Electronics
 *     Mail:	savpekelectronics@gmail.com
 *  
 * File explained:
 *	Library for creating terminal like program over USART.
 */

#ifndef TERM_H_
#define TERM_H_

#include "compiler.h"
#include "preprocessor.h"
#include "usart.h"
#include "gpio.h"
#include "mcu.h"

/* Startup settings for terminal, includes USART settings etc... */
#define TERMINAL_USART 		(&AVR32_USART0)
#define TERMINAL_BAUDRATE	56000
#define TERMINAL_CHARLENGTH	8
#define TERMINAL_PARITYPE	USART_NO_PARITY
#define TERMINAL_STOPBITS	USART_2_STOPBITS
#define TERMINAL_CHANNELMODE USART_NORMAL_CHMODE

#define TERMINAL_USART_RX_PIN        AVR32_USART0_RXD_0_PIN
#define TERMINAL_USART_RX_FUNCTION   AVR32_USART0_RXD_0_FUNCTION
#define TERMINAL_USART_TX_PIN        AVR32_USART0_TXD_0_PIN
#define TERMINAL_USART_TX_FUNCTION   AVR32_USART0_TXD_0_FUNCTION

/* VT100 terminal ASCII marks and special commands. */
#define TERMINAL_NL 0x0D
#define TERMINAL_SPACEBAR 0x20

/*@ Thread function for terminal. */
extern void terminal_thread(void*);

/*@ Inits terminal, reguires that clocks have configured correctly,
 * so don't run this before you have set them up.
 * Basically this function inits serial transmission from MCU.*/
extern void terminal_init(void);

/*@ Print functions for constant and nonconstant data */
extern void terminal_write_line(char* line_string);
#define terminal_write_line_const(str) usart_write_line(TERMINAL_USART, str);

/* Debug functions.
 * These functions prints variables, program states and so on for
 * debug purposes during programming. */
#define PRINT_LONG_WORD 32
#define PRINT_WORD 16
#define PRINT_BYTE 8
extern void terminal_print_bits (int32_t, uint8_t print_length);
extern void terminal_print_hex (int32_t hex_value, uint8_t print_length);

#endif /* SERTERM_H_ */
