/*
 * terminalapi_config.h
 *
 * Created: 10.12.2011 11:45:50
 *  Author: savpek
 */ 


#ifndef TERMINALAPI_CONFIG_H_
#define TERMINALAPI_CONFIG_H_

//#define TERMINALAPI_DEBUG

/* You must include api that is used as output here */
#include "./driver/usart/public/usart.h"
#include "./mcu/inc/mcu.h"

/* Define init function of used output stream */
#define TERMINALAPI_INIT_OUTPUT_DEVICE() usart_init_rs232(MCU_PBA_F);

/* This defines output and read functions for stream that terminal
 * will use. */
#define TERMINALAPI_PRINT_FUNCTION usart_write_line
#define TERMINALAPI_PUTCHAR_FUNCTION usart_putchar
#define TERMINALAPI_GETCHAR_FUNCTION usart_read_char
#define TERMINALAPI_READCHAR_FUNCTION usart_try_read_char

/* This defines maximum length of used command. You cannot type
 * longer line to terminal than this. */
#define TERMINALAPI_CMD_LENGTH 25

#endif /* TERMINALAPI_CONFIG_H_ */