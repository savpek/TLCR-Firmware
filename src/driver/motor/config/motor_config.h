/*
 * motion_config.h
 *
 * Created: 28.8.2011 21:45:56
 *  Author: savpek
 */ 


#ifndef MOTION_CONFIG_H_
#define MOTION_CONFIG_H_

#include "compiler.h"

/*! Settings for PWMA module. 
 * If dision is enabled, PWM freq is
 * CPU_CLK/Division factor. Remember that actual frequency
 * of PWM signal is (CPU_FREG/DIVISION_F)/PERIOD! */
#define MOTOR_PWMA_ENABLE_CLK_DIV 1
#define MOTOR_PWMA_DIVISION_FACTOR 7
#define MOTOR_PWMA_PERIOD 127

/*  Maximum count of motors */
#define MOTOR_MAX_COUNT 2

/*! After step, how long will take before motor will drop it's
 *	power. This because stepper motor hold torgue is unnecessary big and
 *	power consuming with full pwm value. !*/
#define MOTOR_SLEEP_DELAY_MS 2000

/*! How many different PWM values are in one step sequence. */
#define MOTOR_STEP_COUNT 23

/*! How many PBA_FREQ/8 cycles must happen that it counts as millisecond in
 * real world. This is for controls IRQ that toggles PWM values for stepper
 * motors. */
#define MOTOR_PBA_CYCLES_AS_MS 6000

/*! Datatype for single step, idea is to make "list" of PWM values
 * that are used for microstepping. */
typedef struct
{
	int8_t a_pwma_value;
	int8_t b_pwma_value;
} motor_step_t;

/*!! This global variable includes information of each step PWM value.
 *	List is defined in motion_motor_config.c file. !! */
extern const motor_step_t motor_step[];
extern const motor_step_t motor_sleep_step[];

#endif /* MOTION_CONFIG_H_ */