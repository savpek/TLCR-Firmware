/*
 * CProgram1.c
 *
 * Created: 8.8.2011 22:44:13
 *  Author: savpek
 */ 

#include "compiler.h"
#include "terminal.h"
#include "./terminal/common/usart/config/usart_settings.h"
#include "./terminal/common/usart/inc/usart_wb.h"
#include "gpio.h"
#include "./mcu/inc/mcu.h"

/* @Inits serial transmission
 * from MCU. Uses USART driver from Atmels framework.*/
extern void terminal_init()
	{
	/* Map ports for USART, Atmel driver function reguires mapping
	 * in gpio_map_t format. */
	static const gpio_map_t USART_GPIO_MAP =
	   {
	     {USART_RX_PIN, USART_RX_FUNCTION},
	     {USART_TX_PIN, USART_TX_FUNCTION},
		 {USART_CTS_PIN, USART_CTS_FUNCTION},
		 {USART_RTS_PIN, USART_RTS_FUNCTION}
	   };

	/* Assign GPIO to USART. */
	gpio_enable_module(USART_GPIO_MAP,
	                     sizeof(USART_GPIO_MAP) / sizeof(USART_GPIO_MAP[0]));

	/* Initialize USART in RS232 mode. */
	//usart_init_rs232(TERMINAL_USART, &USART_OPTIONS, MCU_PBA_F);
	usart_init_rs232_with_rxdry_irq(MCU_PBA_F);
	}