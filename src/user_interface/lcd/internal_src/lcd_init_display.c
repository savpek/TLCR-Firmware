/*
 * lcd_init_display.c
 *
 * Created: 28.9.2011 16:46:44
 *  Author: savpek
 */ 

#include "compiler.h"
#include "lcd_ctrl_functions.h"
#include "lcd_config.h"
#include "FreeRTOS.h"
#include "task.h"

void _e_routine_with_delay ()
	{
	/* Get enable pin high */
	gpio_set_pin_high(LCD_E_PIN);
	
	/* Wait for while */
	vTaskDelay(1/portTICK_RATE_MS);
	
	/* Set enable pin low */
	gpio_set_pin_low(LCD_E_PIN);
	
	/* Now wait to be sure that lcd device is ready for
	 * new instructions */
	vTaskDelay(1/portTICK_RATE_MS);
	}

/* TESTAA KAIKKI LINJAT 
RS OK
E  OK
RW OK
D4
D5
D6
D7
*/


/*! This function inits LCD display. Function inits
 *  display to 4 data bit mode. */
void lcd_init_display ( void )
	{
	/* When initing display, all pins are outputs since
	 * busy flag cannot be read yet. */
	gpio_configure_pin(LCD_RW_PIN, GPIO_DIR_OUTPUT);
	gpio_configure_pin(LCD_E_PIN, GPIO_DIR_OUTPUT);
	gpio_configure_pin(LCD_RS_PIN, GPIO_DIR_OUTPUT);
	gpio_configure_pin(LCD_D4_PIN, GPIO_DIR_OUTPUT);
	gpio_configure_pin(LCD_D5_PIN, GPIO_DIR_OUTPUT);
	gpio_configure_pin(LCD_D6_PIN, GPIO_DIR_OUTPUT);
	gpio_configure_pin(LCD_D7_PIN, GPIO_DIR_OUTPUT);
	
	/* Begin init cycle, see further information from
	 * displays datasheet. */
	/* Wait for longer than 40ms after VDD > 2.7V !*/
	vTaskDelay(50/portTICK_RATE_MS);
	
	//while(1)
		//{
		//gpio_set_pin_high(LCD_RW_PIN);
		//vTaskDelay(50/portTICK_RATE_MS);
		//gpio_set_pin_low(LCD_RW_PIN);
		//vTaskDelay(50/portTICK_RATE_MS);	
		//}
	
	/* First instruction is DB4=1, DB5=1 */
	gpio_set_pin_high(LCD_D4_PIN);
	gpio_set_pin_high(LCD_D5_PIN);
	
	_e_routine_with_delay ();
		
	/* As datasheet says, here must be >4.1ms delay, so
	 * lets add some more. */
	vTaskDelay(5/portTICK_RATE_MS);
	
	/* Datasheet says that here must be longer than >100us
	 * delay, however _e_routine_with_delay() has 1ms, so
	 * enough for sure. */
	_e_routine_with_delay ();
	
	/* Third instruction is again, DB4 and DB5=1 */
	_e_routine_with_delay ();
	
	/* Now display is set to 4pin data mode and busy flag can
	 * be checked. However, instruction is done with delays only.*/		
	
	/* Function set instruction for 4 bits. */
	gpio_set_pin_low(LCD_D4_PIN);
	gpio_set_pin_high(LCD_D5_PIN);
	
	_e_routine_with_delay ();
	
	/* Set number of display lines and characters,
	 * First part of instruction is still DB5 = 1,
	 * So only need for enable routine again. */
	_e_routine_with_delay ();
	
	/* Second instruction set of display lines and
	 * font is:
	 * DB7, N, number of lines = 2, So bit value is =1
	 * DB6, F, display font, 5x11dots = bit value 1 */
	gpio_set_pin_high(LCD_D7_PIN);
	gpio_set_pin_high(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);
	
	_e_routine_with_delay ();
	
	/* Next we put display off, as datasheet
	 * told us to do. */
	gpio_set_pin_low(LCD_D7_PIN);
	gpio_set_pin_low(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);
	
	_e_routine_with_delay ();
	
	/* Display off, second set. */
	gpio_set_pin_high(LCD_D7_PIN);
	gpio_set_pin_low(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);
	
	_e_routine_with_delay();
	
	/* Display clear, firs set. */
	gpio_set_pin_low(LCD_D7_PIN);
	gpio_set_pin_low(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);

	_e_routine_with_delay();

	/* Display clear, second set. */
	gpio_set_pin_low(LCD_D7_PIN);
	gpio_set_pin_low(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_high(LCD_D4_PIN);
	
	_e_routine_with_delay();
	
	/* Entry mode set, first set. */
	gpio_set_pin_low(LCD_D7_PIN);
	gpio_set_pin_low(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);
	
	_e_routine_with_delay();
	
	/* Entry mode, second set. 
	 * DB5 = cursor dir,
	 * DB4 = Shift display on/off */
	gpio_set_pin_low(LCD_D7_PIN);
	gpio_set_pin_high(LCD_D6_PIN);
	gpio_set_pin_high(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);
	
	_e_routine_with_delay();
	
	/* Set display on */
	gpio_set_pin_low(LCD_D7_PIN);
	gpio_set_pin_low(LCD_D6_PIN);
	gpio_set_pin_low(LCD_D5_PIN);
	gpio_set_pin_low(LCD_D4_PIN);
	
	_e_routine_with_delay();
	
	gpio_set_pin_high(LCD_D7_PIN);
	gpio_set_pin_high(LCD_D6_PIN);
	gpio_set_pin_high(LCD_D5_PIN);
	gpio_set_pin_high(LCD_D4_PIN);
	
	_e_routine_with_delay();
	
	while(1);	
	}