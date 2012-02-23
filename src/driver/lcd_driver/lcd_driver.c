/*
 * lcd_driver.ca
 *
 * Created: 22.2.2012 12:44:05
 *  Author: savpek
 */

#include "compiler.h"
#include "./lcd_driver/public/lcd_driver.h"
#include "./lcd_driver/private/lcd_driver_private.h"
#include "./lcd_driver/config/lcd_driver_config.h"

void lcd_driver_init() {
	
	/* Initial routines to enter 4 bit interface. */
	lcd_driver_write_cmd((1<<LCD_D_DB5)|(1<<LCD_D_DB4)|(1<<LCD_D_DB1)|(1<<LCD_D_DB0));
	lcd_driver_write_cmd((1<<LCD_D_DB5)|(1<<LCD_D_DB4)|(1<<LCD_D_DB1));

	/* Rest of initial routines */
	lcd_driver_write_cmd(LCD_D_FUNCTION_SET_INIT);
	lcd_driver_write_cmd(LCD_D_SET_OFF);
	lcd_driver_write_cmd(LCD_D_CLEAR_DISPLAY);
	lcd_driver_write_cmd(LCD_D_ENTRY_INIT);
	lcd_driver_write_cmd(LCD_D_SET_ON);
}

void lcd_driver_display_on() {
	lcd_driver_write_cmd(LCD_D_SET_ON);
}

void lcd_driver_display_off() {
	lcd_driver_write_cmd(LCD_D_SET_OFF);
	return;
}