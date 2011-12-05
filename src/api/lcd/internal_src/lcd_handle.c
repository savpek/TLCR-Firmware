/*
 * CProgram1.c
 *
 * Created: 28.9.2011 12:54:15
 *  Author: savpek
 */ 

#include "compiler.h"
#include "lcd.h"

lcd_t lcd_handle =
	{
	.rotate_delay_ms = 0,
	.move_delay_ms = 0,
	.rotate_direction = LCD_COUNTER_CLOCKW,
	.move_direction = LCD_LEFT,
	.picture_delay_ms = 0,
	.selected_program_number = 0,
	.display_mode = LCD_SPEED_AND_DIRECTION
	};