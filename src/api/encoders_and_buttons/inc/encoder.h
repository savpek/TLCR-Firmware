/*
 * encoder.h
 *
 * Created: 29.9.2011 17:45:30
 *  Author: savpek
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#include "compiler.h"
#include "encoder_config.h"

/* This struct contains information about
 * certain encoder. This is used wia globan handle
 * since encoder states are written in ISR (extint). */
typedef struct  
{
	/* A pin has interrupt routines. */
	const uint8_t a_pin;
	
	/* B pin state is polled when interrupt occurs. */
	const uint8_t b_pin;
	
	/* This contains step count of encoder. Increments */
	uint32_t step_count;
} encoderc_t;

/* This handle is way to init and control
 * encoders. Encoders works through interrupt
 * routines defined in encoder_isr.h in internal_src
 * folder. */
extern encoderc_t encoder_handle[ENCODER_COUNT];

/* This function inits all encoders. Inits ISR routines
 * and IO settings. */
extern void encoder_init_all( void );

#endif /* ENCODER_H_ */