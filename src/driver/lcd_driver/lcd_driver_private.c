/*
 * lcd_driver_private.c
 *
 * Created: 22.2.2012 12:46:51
 *  Author: savpek
 */ 

#include "compiler.h"

static void lcd_driver_write_cmd_body(uint32_t cmd) {
	
}
void (*lcd_driver_write_cmd)(uint32_t cmd) = &lcd_driver_write_cmd_body;