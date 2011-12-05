/*
 * lcd.h
 *
 * Created: 7.9.2011 17:47:01
 *  Author: savpek
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "lcd_config.h"

/* Enums for make reading easier */
typedef enum 
	{
	/* In this mode, display will show:
	 * SPEEDX MM/H ->
	 * SPEEDR ANGLE/H DIR */
	LCD_SPEED_AND_DIRECTION,
	
	/* At this mode device shows picture
	 * count shortly and then it will return
	 * to SPEED_AND_DIRECTION display. */
	LCD_SHOW_PICTURE_COUNT_SHORTLY,
	
	/* At this display select program to run. */
	LCD_SELECT_PROGRAM,
	
	/* Show current program number that is running.
	 * Show next event below. */
	LCD_PROGRAM_RUNNING,
	
	/* Stuck in screen:
	 * TERMINAL
	 * CONTROL */
	LCD_TERMINAL_HAS_CTRL
	} lcd_mode_t;

/* Directions that motors are currently moving */
typedef enum
	{
		LCD_LEFT,
		LCD_RIGHT,
		LCD_COUNTER_CLOCKW,
		LCD_CLOCKW
	} lcd_dir_t;

/*! This struct has all parameters needed to build
 *  string that display shows. */
typedef struct {
	/* These are speed parameters for SPEED_AND_DIRECTION
	 * display.*/
	uint32_t rotate_delay_ms;
	uint32_t move_delay_ms;
	
	lcd_dir_t rotate_direction;
	lcd_dir_t move_direction;
	
	/* These are paremeters for
	 * SHOW_PICTURE_COUNT_SHORTLY display. */
	uint32_t picture_delay_ms;
	
	/* Parameters for program currently running. */
	uint8_t selected_program_number;
	
	/* Program thread keeps this updated, tells which event
	 * will be driven next. */
	char *next_program_event[8];
	
	/* This selects display mode. */
	lcd_mode_t display_mode;
} lcd_t;

/*  LCD handle, defined in lcd_handle.c in internal_src. */

/*@ This function updates lcd with parameters given. See truct
 *  above to understand how it should be changed to get correct display at time. 
 *	Before use, you update*/
extern void lcd_update(void);

/*@ This thread handles lcd display. We use own thread for this
 *  because there are delays and such which may cause loss of real
 *  time effect if this is controlled directly through user interface thread. */
extern void lcd_thread(void *params);

#endif /* LCD_H_ */