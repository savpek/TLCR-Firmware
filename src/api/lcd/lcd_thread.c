/*
 * lcd_thread.c
 *
 * Created: 7.9.2011 19:02:59
 *  Author: savpek
 */ 

#include "compiler.h"
#include "lcd.h"
#include "lcd_ctrl_functions.h"

/* This threads updates lcd content. */
void lcd_thread (void* params)
	{		
	lcd_init_display();
	
	while(1);
	}