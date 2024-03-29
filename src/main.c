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
#include "./errorh/inc/errorh.h"
#include "./app/terminal/inc/terminal.h"
#include "./terminalapi/inc/terminalapi.h"
#include "./storage/inc/storage.h"
#include "FreeRTOS.h"
#include "task.h"
#include "./motion/inc/motion.h"
//#include "encoders_and_buttons_isr.h"
//#include "terminal_usart_settings.h"
//#include "delay.h"
#include "./flashmem/inc/flashmem.h"
#include "./flashmem/inc/flashmem.h"
#include "./gpio/public/gpio.h"
#include "./str/public/str.h"
#include "./driver/usart/public/usart.h"
#include "./scriptapi/inc/scriptapi.h"
#include "./driver/motor/inc/motor.h"
#include "./ioapi/public/ioapi.h"
#include "./print/public/print.h"

/*
 * Here we define test framework!
 */
#define RUN_TESTS
#define RUN_FIRMWARE

#include "./utest/public/utest.h"

#include "./init_drivers/public/init_drivers.h"

/* Include all test files */

static void run_tests() {
	RUN_TEST_GROUP(str);
	RUN_TEST_GROUP(print);
	RUN_TEST_GROUP(read);
	RUN_TEST_GROUP(ioapi);
	RUN_TEST_GROUP(lcd_driver_private);
	RUN_TEST_GROUP(lcd_driver);
}

int main(void)
	{
	#ifdef RUN_TESTS
		init_drivers();
		run_utest_main(run_tests);
	#endif

	init_drivers();

	ioapi_output_low(AVR32_PIN_PA08);

	INTC_init_interrupts();

	/* Set printing function for error libraries */
	ERRORH_SET_PRINT_HANDLER(usart_write_line);
	delay_init(48000000);
	//encoder_init_all();
	//button_init_all();
	//flashmem_init();

	//motion_init();
//	terminal_thread(0);
	/* Add terminal to FreeRTOS run list */
	//xTaskCreate(terminal_thread, "terminal", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);

	/* LCD thread */
	//xTaskCreate(lcd_thread, "lcd_thread", 1000, NULL, FREERTOS_PRIORITY_NORMAL, NULL);

	/* Begin FreeRtos scheduling */
//	vTaskStartScheduler();

	/* If program reaches this point, FreeRtos have crashed, badly. */
	while(1);
	}