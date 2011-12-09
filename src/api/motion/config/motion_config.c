/*
 * motion_motor_config.c
 *
 * Created: 28.8.2011 15:50:30
 *  Author: savpek
 */ 

#include "motion_config.h"
#include "compiler.h"

const motorc_step_t motion_step_sequence[] = {
 /*  A-coil     B-coil		<-PWMA value */
	{-127,		-127},		//STEP 0
	{127,		-127},		//STEP 1
	{127,		127},		//STEP 2
	{-127,		127},		//STEP 3
};

const motorc_step_t motion_step_sleep_sequence[] = {
 /*  A-coil     B-coil		<-PWMA value */
	{-60,		-60},		//STEP 0
	{60,		-60},		//STEP 1
	{60,		60},		//STEP 2
	{-60,		60},		//STEP 3
};

/* Settings for motor which rotates camera platform */
volatile motorc_t motion_handle[] =
	{
	/* MOTION_ID_XMOTOR (0): ... */
		{
		/* Define physical pins for PWMA */
		.coil_a_pos_pwma_pin = AVR32_PWMA_28_PIN,	/* Often RED wire*/
		.coil_a_neg_pwma_pin = AVR32_PWMA_29_PIN,	/* Often BLUE wire*/
		.coil_b_pos_pwma_pin = AVR32_PWMA_30_PIN,	/* Often YELLOW wire*/
		.coil_b_neg_pwma_pin = AVR32_PWMA_31_PIN,	/* Often WHITE wire*/

		/* Define PWMA channel for each coil pin */
		.coil_a_pos_pwma_channel = 28,
		.coil_a_neg_pwma_channel = 29,
		.coil_b_pos_pwma_channel = 30,
		.coil_b_neg_pwma_channel = 31,
		.pwma_function = AVR32_PWMA_15_FUNCTION,
	
		/* We don't want that motor suddenly moves in beginning. */
		.motor_direction = STOP,
	
		/* Set initial value to zero. */
		.step_count = 0,
		.step_delay_ms = 0
		},
	/* MOTION_ID_RMOTOR (1): ... */
		{
		/* Define physical pins for PWMA */
		.coil_a_pos_pwma_pin = AVR32_PWMA_24_PIN,	/* Often RED wire*/
		.coil_a_neg_pwma_pin = AVR32_PWMA_25_PIN,	/* Often BLUE wire*/
		.coil_b_pos_pwma_pin = AVR32_PWMA_26_PIN,	/* Often YELLOW wire*/
		.coil_b_neg_pwma_pin = AVR32_PWMA_27_PIN,	/* Often WHITE wire*/

		/* Define PWMA channel for each coil pin */
		.coil_a_pos_pwma_channel = 24,
		.coil_a_neg_pwma_channel = 25,
		.coil_b_pos_pwma_channel = 26,
		.coil_b_neg_pwma_channel = 27,
		.pwma_function = AVR32_PWMA_15_FUNCTION,
	
		/* We don't want that motor suddenly moves in beginning. */
		.motor_direction = STOP,
	
		/* Set initial value to zero. */
		.step_count = 0,
		.step_delay_ms = 0
		}
	};