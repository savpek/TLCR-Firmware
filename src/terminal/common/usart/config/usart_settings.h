/*
 * terminal_usart_settings.h
 *
 * Created: 26.9.2011 22:04:25
 *  Author: savpek
 */ 
#ifndef USART_SETTINGS_H_
#define USART_SETTINGS_H_

/* Startup settings for terminal, includes USART settings etc... */
#define USART_POINTER 		(&AVR32_USART0)
#define USART_BAUDRATE	56000
#define USART_CHARLENGTH	8
#define USART_PARITYPE	USART_NO_PARITY
#define USART_STOPBITS	USART_2_STOPBITS
#define USART_CHANNELMODE USART_NORMAL_CHMODE

#define USART_RX_PIN        AVR32_USART0_RXD_0_PIN
#define USART_RX_FUNCTION   AVR32_USART0_RXD_0_FUNCTION
#define USART_TX_PIN        AVR32_USART0_TXD_0_PIN
#define USART_TX_FUNCTION   AVR32_USART0_TXD_0_FUNCTION
#define USART_CTS_PIN       AVR32_USART0_CTS_0_0_PIN
#define USART_CTS_FUNCTION  AVR32_USART0_CTS_0_0_FUNCTION
#define USART_RTS_PIN       AVR32_USART0_RTS_0_PIN
#define USART_RTS_FUNCTION  AVR32_USART0_RTS_0_FUNCTION

/* This defines how many attempts usart makes to receive character
 * before it sends XON character. */
#define USART_XON_DELAY		1000

/* This defines how many items are in fifo before
 * usart trys pause transmission with XOFF */
#define USART_XOFF_DELAY	3

#endif /* USART_SETTINGS_H_ */