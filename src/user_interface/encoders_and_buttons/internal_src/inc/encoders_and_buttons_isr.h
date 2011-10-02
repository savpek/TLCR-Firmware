/*
 * encoder_isr.h
 *
 * Created: 29.9.2011 23:21:21
 *  Author: savpek
 */ 


#ifndef ENCODER_ISR_H_
#define ENCODER_ISR_H_

#include "compiler.h"
#include "encoder.h"
#include "gpio.h"

extern void encoders_and_buttons_isr(void);
	
#endif /* ENCODER_ISR_H_ */