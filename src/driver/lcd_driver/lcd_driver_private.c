/*
 * lcd_driver_private.c
 *
 * Created: 22.2.2012 12:46:51
 *  Author: savpek
 */

#include "compiler.h"
#include "./ioapi/public/ioapi.h"
#include "./lcd_driver/config/lcd_driver_config.h"

static void e_pin_routine() {
	ioapi_output_high(IOPIN_LCD_E);
	ioapi_output_low(IOPIN_LCD_E);
}

static void set_e_pin_low_safeguard() {
	ioapi_output_low(IOPIN_LCD_E);
}

static void set_write_char_mode () {
	ioapi_output_high(IOPIN_LCD_RS);
}

/* With this (RS pin low) you write everything
 * else except characters to display memory. */
static void set_write_common_mode () {
	ioapi_output_low(IOPIN_LCD_RS);
}

static void write_lowest_4_data_bits(uint32_t bits) {
	bits = bits & 0xF;
	if((bits&(1<<0)) == 0)
		ioapi_output_low(IOPIN_LCD_DB4);
	else
		ioapi_output_high(IOPIN_LCD_DB4);

	if((bits&(1<<1)) == 0)
		ioapi_output_low(IOPIN_LCD_DB5);
	else
		ioapi_output_high(IOPIN_LCD_DB5);

	if((bits&(1<<2)) == 0)
		ioapi_output_low(IOPIN_LCD_DB6);
	else
		ioapi_output_high(IOPIN_LCD_DB6);

	if((bits&(1<<3)) == 0)
		ioapi_output_low(IOPIN_LCD_DB7);
	else
		ioapi_output_high(IOPIN_LCD_DB7);
}

static void set_rs_pin_correctly(uint32_t cmd) {
	if((cmd&LCD_D_SEND_CHAR_MASK) == LCD_D_SEND_CHAR_MASK) {
		ioapi_output_high(IOPIN_LCD_RS);
	}
	else {
		ioapi_output_low(IOPIN_LCD_RS);
	}
}

static void lcd_driver_write_cmd_body(uint32_t cmd) {
		set_e_pin_low_safeguard();
		set_rs_pin_correctly(cmd);
		write_lowest_4_data_bits(cmd>>4);
		e_pin_routine();
		write_lowest_4_data_bits(cmd);
		e_pin_routine();
}
void (*lcd_driver_write_cmd)(uint32_t) = lcd_driver_write_cmd_body;