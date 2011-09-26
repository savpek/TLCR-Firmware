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

#include "usart.h"

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

/*@ Thread function for terminal (compatible with FreeRTOS). */
extern void terminal_thread(void*);

/*@ Inits terminal, reguires that clocks have configured correctly,
 * so don't run this before you have set them up.
 * Basically this function inits serial transmission from MCU.*/
extern void terminal_init(void);

#endif /* SERTERM_H_ */
