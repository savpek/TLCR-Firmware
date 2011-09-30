/*
 * encoder.h
 *
 * Created: 29.9.2011 17:45:30
 *  Author: savpek
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#include "compiler.h"

/* This struct contains information about
 * certain encoder. This is used wia globan handle
 * since encoder states are written in ISR (extint). */
enum struct  
{
	/* A pin has interrupt routines. */
	uint8_t a_pin;
	uint8_t a_function;
	
	/* B pin state is polled when interrupt occurs. */
	uint8_t b_pin;
	uint8_t b_function;
	
	/* Define IRQ line of EIC module. */
	uint8_t irq;
	
	/* This contains step count of encoder. Increments */
	uint32_t cycle_count;
} encoderc_t;

/* This handle is way to init and control
 * encoders. Encoders works through interrupt
 * routines defined in encoder_isr.h in internal_src
 * folder. */
extern encoder_handle[];

/* This function inits all encoders. Inits ISR routines
 * and IO settings. */
extern void encoder_init_all();

#endif /* ENCODER_H_ */