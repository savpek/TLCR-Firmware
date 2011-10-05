/*
 * button_handle.c
 *
 * Created: 2.10.2011 14:35:56
 *  Author: savpek
 */ 

#include "compiler.h"
#include "button.h"
#include "button_config.h"

button_t button_handle[BUTTON_COUNT] =
	{
	/* ENCODER 1 BUTTON */
		{
		.col_pol_pin = AVR32_PIN_PA12,
		.row_int_pin = AVR32_PIN_PA16,
		.button_pressed_flag = 0
		}
	/* ENCODER 2 BUTTON */
	
	/* ENCODER 3 BUTTON */
	};