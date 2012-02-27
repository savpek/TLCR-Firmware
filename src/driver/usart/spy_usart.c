/*
 * usart_spy.c
 *
 * Created: 24.2.2012 0:29:24
 *  Author: savpek
 */

#include "compiler.h"
#include "utest/public/utest.h"

volatile char spy_usart_buffer[50] = {0};
volatile uint8_t buffer_idx = 0;

void spy_usart_putchar(char a) {
	if(buffer_idx >= 48) {
		TEST_FAIL_MESSAGE("SPY_USART buffer overrun!");
		return;
	}

	spy_usart_buffer[buffer_idx] = a;
	spy_usart_buffer[buffer_idx+1] = '\0';

	buffer_idx++;
}

void spy_usart_reset_buffer( void ) {
	spy_usart_buffer[0] = '\0';
	buffer_idx = 0;
}