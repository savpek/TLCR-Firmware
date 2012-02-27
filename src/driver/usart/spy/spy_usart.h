/*
 * usart_spy.h
 *
 * Created: 24.2.2012 23:40:27
 *  Author: savpek
 */


#ifndef USART_SPY_H_
#define USART_SPY_H_

extern volatile char spy_usart_buffer[];

extern void spy_usart_putchar(char a);

extern void spy_usart_reset_buffer( void);

#endif /* USART_SPY_H_ */