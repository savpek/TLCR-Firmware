/*
 * print_config.h
 *
 * Created: 24.2.2012 21:44:10
 *  Author: savpek
 */


#ifndef PRINT_CONFIG_H_
#define PRINT_CONFIG_H_

#include "./usart/public/usart.h"

/*! This defines output stream which print api uses. */
#define PRINT_CHAR_STREAMF(a) usart_putchar(a)

#endif /* PRINT_CONFIG_H_ */