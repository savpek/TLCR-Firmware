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

/* Internal */
#include "./terminal/common/usart/inc/usart_wb.h"

/*@ Thread function for terminal (compatible with FreeRTOS). */
extern void terminal_thread(void*);

/*@ Inits terminal, reguires that clocks have configured correctly,
 * so don't run this before you have set them up.
 * Basically this function inits serial transmission from MCU.*/
extern void terminal_init(void);

#endif /* SERTERM_H_ */
