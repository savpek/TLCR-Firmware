/*! @brief Settings for TLCR REVISION 1 HW
 *	
 *	@file	usart_config.h
 *	@author savpek							*/


#ifndef USART_CONFIG_H_
#define USART_CONFIG_H_

/*! @brief Startup settings for usar driver. */

/*! @def points to USART register addr in MCU memory. */
#define INITD_USART_POINTER 	(&AVR32_USART0)

#define INITD_USART_BAUDRATE	9600
#define INITD_USART_CHARLENGTH	8
#define INITD_USART_PARITYPE	USART_NO_PARITY
#define INITD_USART_STOPBITS	USART_2_STOPBITS
#define INITD_USART_CHANNELMODE USART_NORMAL_CHMODE

#endif /* USART_CONFIG_H_ */