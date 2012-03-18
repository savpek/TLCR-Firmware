/*
 * usart_spy.c
 *
 * Created: 24.2.2012 0:29:24
 *  Author: savpek
 */

#include "compiler.h"
#include "utest/public/utest.h"

char spy_usart_buffer_tx[50] = {0};
char spy_usart_buffer_rx[50] = {0};
uint8_t buffer_idx_tx = 0;
uint8_t buffer_idx_rx = 0;

void spy_usart_putchar(char a) {
	if(buffer_idx_tx >= 48) {
		TEST_FAIL_MESSAGE("SPY_USART buffer overrun!");
		return;
	}

	spy_usart_buffer_tx[buffer_idx_tx] = a;
	spy_usart_buffer_tx[buffer_idx_tx+1] = '\0';

	buffer_idx_tx++;
}
void (*backup_usart_putchar)(char a) = NULL;

void spy_usart_reset_tx( void ) {
	spy_usart_buffer_tx[0] = '\0';
	buffer_idx_tx = 0;
}

void spy_usart_put_to_rx(char * this_str) {
	int i = 0;
	for (i = 0; this_str[i] != 0; i++) {
		spy_usart_buffer_rx[i] = this_str[i];
		buffer_idx_rx = i+1;
	}
	spy_usart_buffer_rx[i+1] = '\0';
}

char spy_usart_read_char ( void) {
	if (buffer_idx_rx > 0) {
		buffer_idx_rx--;
		return spy_usart_buffer_rx[buffer_idx_rx];
	}
	return '\0';
}
char (*backup_usart_read_char)(void) = NULL;

errorc_t spy_usart_try_read_char( char *c) {
	if (buffer_idx_rx > 0) {
		buffer_idx_rx--;
		*c = spy_usart_buffer_rx[buffer_idx_rx];
		return EC_SUCCESS;
	}
	*c = '\0';
	return EC_USART_RX_EMPTY;
}
errorc_t (*backup_usart_try_read_char)( char *c) = NULL;