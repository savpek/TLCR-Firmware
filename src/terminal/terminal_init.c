/*
 * CProgram1.c
 *
 * Created: 8.8.2011 22:44:13
 *  Author: savpek
 */ 

#include "compiler.h"
#include "terminal.h"
#include "terminal_usart_settings.h"
#include "usart.h"
#include "gpio.h"
#include "mcu.h"

/* @Inits serial transmission
 * from MCU. Uses USART driver from Atmels framework.*/
extern void terminal_init()
	{
	/* Settings for usart initialization. Atmels USART-driver
	 * reguires settings in usart_options_t type struct.	*/
	static const usart_options_t USART_OPTIONS =
	   {
	     .baudrate     = TERMINAL_BAUDRATE,
	     .charlength   = TERMINAL_CHARLENGTH,
	     .paritytype   = TERMINAL_PARITYPE,
	     .stopbits     = TERMINAL_STOPBITS,
	     .channelmode  = TERMINAL_CHANNELMODE
	   };

	/* Map ports for USART, Atmel driver function reguires mapping
	 * in gpio_map_t format. */
	static const gpio_map_t USART_GPIO_MAP =
	   {
	     {TERMINAL_USART_RX_PIN, TERMINAL_USART_RX_FUNCTION},
	     {TERMINAL_USART_TX_PIN, TERMINAL_USART_TX_FUNCTION},
		 {TERMINAL_USART_CTS_PIN, TERMINAL_USART_CTS_FUNCTION},
		 {TERMINAL_USART_RTS_PIN, TERMINAL_USART_RTS_FUNCTION}
	   };

	/* Assign GPIO to USART. */
	gpio_enable_module(USART_GPIO_MAP,
	                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

	/* Initialize USART in RS232 mode. */
	//usart_init_rs232(TERMINAL_USART, &USART_OPTIONS, MCU_PBA_F);
	usart_init_rs232(TERMINAL_USART, &USART_OPTIONS, MCU_PBA_F);
	}