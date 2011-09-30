/*
 * encoder_handle.c
 *
 * Created: 29.9.2011 23:59:46
 *  Author: savpek
 */ 

#include "compiler.h"
#include "eic.h"

/* This struct contains information about
 * certain encoder. This is used via global handle
 * since encoder states are written in ISR (EXTINT). */
encoderc_t encoder_handle[] = 
	{
	/* ENCODER 1 */
		{
		.a_pin =  AVR32_EIC_EXTINT_1_PIN,
		.a_function AVR32_EIC_EXTINT_1_FUNCTION,
		.b_pin AVR32_PIN_PA11= ,
		
		.irq = EXT_INT1,
		
		.cycle_count = 0
		},
	/* ENCODER 2 */
	
	/* ENCODER 3 */			
	};	