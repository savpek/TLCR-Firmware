/*
 * motor.h
 *
 * Created: 12.12.2011 12:45:32
 *  Author: savpek
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "compiler.h"

#include "./errorh/inc/errorh.h"
#include "./driver/motor/config/motor_config.h"

typedef enum
	{
	MOTOR_STOP = 0,
	MOTOR_CW = 1,
	MOTOR_COUNTERCW = -1
	}motor_dir_t;

typedef struct motor_t
	{
	/* These defines physical pins which stepper motor is attached. */
	const uint8_t a_pos_pwma_pin;		/* Often RED wire*/
	const uint8_t a_neg_pwma_pin;		/* Often BLUE wire*/
	const uint8_t b_pos_pwma_pin;		/* Often YELLOW wire*/
	const uint8_t b_neg_pwma_pin;		/* Often WHITE wire*/

	/* Define PWMA channel for each coil pin */
	const uint8_t a_pos_pwma_chan;
	const uint8_t a_neg_pwma_chan;
	const uint8_t b_pos_pwma_chan;	
	const uint8_t b_neg_pwma_chan;
	
	/* This should be common for all PWMA channels. */
	const uint8_t pwma_function;
	
	
	/* Tells direction of motor. */
	motor_dir_t direction;
	
	/* This tells how long is delay between steps */
	uint32_t delay;
	
	/* Current position in step value list. */
	int32_t step;
	
	/* This counts how many ms have gone since last step. */
	uint32_t delay_counter;
	} motor_t;

/* This inits current motor, adds it to motor list.
 * If motor array is full, returns EC_FULL, otherwise
 * EC_SUCCESS. */
extern errorc_t motor_init(volatile motor_t *motor_handle);

extern void motor_step_delay(volatile motor_t *motor_handle, uint32_t step_delay, motor_dir_t direction);

#endif /* MOTOR_H_ */