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
#include "./terminal/inc/terminal.h"
#include "./mcu/inc/mcu.h"
#include "./storage/inc/storage.h"
#include "FreeRTOS.h"
#include "task.h"
#include "motion.h"
#include "lcd.h"
#include "encoder.h"
//#include "encoders_and_buttons_isr.h"
#include "button.h"
//#include "terminal_usart_settings.h"
//#include "delay.h"

int main(void)
	{
	mcu_init_clocks();
	INTC_init_interrupts();
			
	delay_init(48000000);

	terminal_init();
//	storage_init();
	//encoder_init_all();
	//button_init_all();
//	motion_init();

	/* Add terminal to FreeRTOS run list */
	xTaskCreate(terminal_thread, "terminal", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);

	/* LCD thread */
	//xTaskCreate(lcd_thread, "lcd_thread", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);
	
	/* Begin FreeRtos scheduling */
	vTaskStartScheduler();
	
	/* If program reaches this point, FreeRtos have crashed, badly. */
	while(1);
	}
