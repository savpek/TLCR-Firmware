/*
 * main.c
 *
 *  Created on: 27.7.2011
 *      Author: savpek
 */

/* ASF */
#include "compiler.h"
#include "intc.h"
#include "delay.h"
#include "./api/errorh/inc/errorh.h"
#include "./app/terminal/inc/terminal.h"
#include "./api/mcu/inc/mcu.h"
#include "./api/storage/inc/storage.h"
#include "FreeRTOS.h"
#include "task.h"
#include "./api/motion/inc/motion.h"
//#include "encoders_and_buttons_isr.h"
//#include "terminal_usart_settings.h"
//#include "delay.h"
#include "./api/flashmem/inc/flashmem.h"
#include "./api/flashmem/inc/flashmem.h"
#include "gpio.h"
#include "./api/chrstr/inc/chrstr.h"
#include "./driver/usart/inc/usart.h"

int main(void)
	{
	mcu_init_clocks();
	INTC_init_interrupts();

	errorh_set_print_handler(usart_write_line);
	
	delay_init(48000000);
	terminal_init();
//	storage_init();
	//encoder_init_all();
	//button_init_all();
	flashmem_init();
	
	flashmem_write_uint8_t(10, 0xAA);
	
	uint8_t char_string[10] = {0};
	chrstr_uint32_to_hex_str(0x1E1E1E1E, char_string);
	usart_write_line(char_string);
	
	/* Add terminal to FreeRTOS run list */
	xTaskCreate(terminal_thread, "terminal", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);

	/* LCD thread */
	//xTaskCreate(lcd_thread, "lcd_thread", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);
	
	/* Begin FreeRtos scheduling */
	vTaskStartScheduler();
	
	/* If program reaches this point, FreeRtos have crashed, badly. */
	while(1);
	}
