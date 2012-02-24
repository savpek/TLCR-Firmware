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
#include "./driver/usart/public/usart.h"

/*@ Thread function for terminal (compatible with FreeRTOS). */
extern void terminal_thread(void* arguments);

#endif /* SERTERM_H_ */
