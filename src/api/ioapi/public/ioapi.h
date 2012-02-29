/*
 * ioapi.h
 *
 * Created: 29.2.2012 9:01:20
 *  Author: savpek
 */ 


#ifndef IOAPI_H_
#define IOAPI_H_

#include "compiler.h"

typedef enum {
	IOAPI_PIN_HIGH =		0x01,
	IOAPI_PIN_LOW =			0x02,
	IOAPI_PIN_INPUT =		0x04,
	IOAPI_PIN_OUTPUT =		0x08,
	IOAPI_PIN_HIVALUE =		~0,
	IOAPI_PIN_LOVALUE =		0,
	IOAPI_PWM_OFF =			0
} ioapi_t;

errorc_t iopapi_pin( uint8_t pin_number, const ioapi_t pin_state );

uint32_t ioapi_pin_value( uint8_t pin_number );

errorc_t ioapi_pwm( uint8_t pin_number, uint32_t pwm_percent );

#endif /* IOAPI_H_ */