/*
 * usart_spy.h
 *
 * Created: 24.2.2012 23:40:27
 *  Author: savpek
 */


#ifndef USART_SPY_H_
#define USART_SPY_H_

extern volatile char spy_usart_buffer_tx[];

extern void spy_usart_putchar(char a);
extern void (*backup_usart_putchar)(char a);

extern void spy_usart_reset_tx( void);

extern char spy_usart_read_char( void);
extern void (*backup_usart_read_char)(char a);

extern char spy_usart_try_read_char( void);
extern void (*backup_usart_try_read_char)(char a);

void spy_usart_put_to_rx(char * this_str);
#endif /* USART_SPY_H_ */