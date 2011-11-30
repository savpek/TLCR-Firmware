/*
 * buttons.h
 *
 * Created: 2.10.2011 11:49:35
 *  Author: savpek
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "compiler.h"
#include "button_config.h"
#include "./errorh/inc/errorh.h"

/* Briefly, buttons are in array. In steady mode
 * all poll lines are in ground and all interrupt lines
 * are pulled up with internal pull up resistors. When button
 * is pressed, interrupt line voltage drops down. However, when
 * interrupt occurs, we can only know that it is one button of
 * row of interrupt line. Now we must set all but one poll line in
 * high impedance state, check was that line correct, go to next, check
 * and so on. */

/* This struct contains basic information of single button. */
typedef struct
	{
	/* Pols pins are counted as columns in
	 * button array. */
	const uint8_t col_pol_pin;
	
	/* Interrupt lines are counted as rows in
	 * button array. */
	const uint8_t row_int_pin;
	
	/* This flag is 1 when button have been pressed,
	 * 0 when not. */
	uint8_t button_pressed_flag;
	} button_t;

extern button_t button_handle[BUTTON_COUNT];

/* This function inits all buttons. */
extern void button_init_all( void );

/* This function checks that is button pressed down atm. 
 * Usefull for limits switches for example, when you need
 * to check that is button still down. */
extern errorc_t button_is_it_pressed_atm(button_t button_handle_struct);

#endif /* BUTTONS_H_ */