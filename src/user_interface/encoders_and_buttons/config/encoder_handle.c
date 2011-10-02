/*
 * encoder_handle.c
 *
 * Created: 29.9.2011 23:59:46
 *  Author: savpek
 */ 

#include "compiler.h"
#include "encoder.h"

/* This struct contains information about
 * certain encoder. This is used via global handle
 * since encoder states are written in ISR (EXTINT). */
encoderc_t encoder_handle[ENCODER_COUNT] = 
	{
	/* ENCODER 1 */
		{
		.a_pin =  AVR32_PIN_PA09,
		.b_pin = AVR32_PIN_PA10,

		.step_count = 0
		},
	/* ENCODER 2 */
	
	/* ENCODER 3 */			
	};	