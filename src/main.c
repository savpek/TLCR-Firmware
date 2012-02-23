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
#include "./api/terminalapi/inc/terminalapi.h"
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
#include "./gpio/public/gpio.h"
#include "./api/chrstr/inc/chrstr.h"
#include "./driver/usart/inc/usart.h"
#include "./api/scriptapi/inc/scriptapi.h"
#include "./driver/motor/inc/motor.h"

/*
 * Here we define test framework!
 */
#define RUN_TESTS
#define RUN_FIRMWARE

#include "./utest/public/utest.h"

/* Include all test files */

static void run_tests() {
	RUN_TEST_GROUP(lcd_driver);
}

int main (void) {
	mcu_init_clocks();
	terminalapi_init();
	usart_write_line("\n\r\n\r UNIT TEST RUN:\r\n");
	UnityMain(run_tests);
}

#if 0

int main(void)
	{
	mcu_init_clocks();
//	INTC_init_interrupts();

	/* Set printing function for error libraries */
	ERRORH_SET_PRINT_HANDLER(usart_write_line);
//	delay_init(48000000);
	//encoder_init_all();
	//button_init_all();
	flashmem_init();

	motion_init();

	/* Add terminal to FreeRTOS run list */
	xTaskCreate(terminal_thread, "terminal", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);

	/* LCD thread */
	//xTaskCreate(lcd_thread, "lcd_thread", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);

	/* Begin FreeRtos scheduling */
	vTaskStartScheduler();

	/* If program reaches this point, FreeRtos have crashed, badly. */
	while(1);
	}

#endif