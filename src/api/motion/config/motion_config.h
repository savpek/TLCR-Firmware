/*
 * motion_config.h
 *
 * Created: 28.8.2011 21:45:56
 *  Author: savpek
 */ 


#ifndef MOTION_CONFIG_H_
#define MOTION_CONFIG_H_

#include "compiler.h"

/*! Define motor ID:s, these are used as handles which gives
 * gate to control for motors from elsewhere. EG: motion_set_speed(ID, SPEED) 
 * from motion_handle.h 
 * These must be in increasing order, starting from 0. */
#define MOTION_RMOTOR_ID	1
#define MOTION_XMOTOR_ID	0

/*!! These constants tells how long distance device moves during one step from
 *   stepper motors. !!*/
#define MOTION_UM_PER_STEP_XMOTOR 100 /* Moves 100um = 0.1mm during one step */
#define MOTION_1_100_OF_DEG_PER_STEP_RMOTOR 100 /* Moves 1 degree during one step */

/*!! This is constant that tells how many ms is in hour */
#define MOTION_MS_PER_HOUR 60*60*1000 

/* This must be set to  correct value! This means as name says, last
 * ID is last motor ID from ID list. Used as loop limit in motion_irq() */
#define MOTION_LAST_MOTOR_ID 1

/*!! PWMA pins are configured in motion_config.c file directly to
 * motor parameters !!*/

/*! Settings for PWMA module. 
 * If dision is enabled, PWM freq is
 * CPU_CLK/Division factor. Remember that actual frequency
 * of PWM signal is (CPU_FREG/DIVISION_F)/PERIOD! */
#define MOTION_PWMA_ENABLE_CLK_DIV 1
#define MOTION_PWMA_DIVISION_FACTOR 15
#define MOTION_PWMA_PERIOD 127

/*! After step, how long will take before motor will drop it's
 *	power. This because stepper motor hold torgue is unnecessary big and
 *	power consuming with full pwm value. !*/
#define MOTION_SLEEP_DELAY_MS 2000

/*! How many different PWM values are in one step sequence. */
#define MOTION_MICROSTEP_COUNT 4

/*! How many PBA_FREQ/8 cycles must happen that it counts as millisecond in
 * real world. This is for controls IRQ that toggles PWM values for stepper
 * motors. */
#define MOTION_PBA_CYCLES_AS_MS 3000

/*! This is type which is used to control selected motor direction */
typedef enum motorc_dir_t{
	LEFT = -1,
	RIGHT = 1,
	STOP = 0,
	STOP_LIMITSW = 0,
	COUNTER_CLOCKW = -1,
	CLOCKW = 1
} motorc_dir_t;

/*! This is type which tells which module of software is currently controlling
 * device. Basic idea is that terminal can get control of device when it is necessary. When
 * terminal is having control UI interface is disabled until control is disabled from terminal or
 * device is reseted. Another possible controller is program, which is actually an thread that
 * updates required parameters real time based on events saved on flash memory. */
typedef enum motorc_ctrl_unit_t {
	FREE = 0,
	TERMINAL = 1,
	USER_INTERFACE = 2,
	PROGRAM = 3
} motorc_ctrl_unit_t;

/*! Datatype for single step, idea is to make "list" of PWM values
 * that are used for microstepping. */
typedef struct
{
	int8_t coil_a_pwma_value;
	int8_t coil_b_pwma_value;
} motorc_step_t;

/*!! This global variable includes information of each step PWM value.
 *	List is defined in motion_motor_config.c file. !! */
extern const motorc_step_t motion_step_sequence[];
extern const motorc_step_t motion_step_sleep_sequence[];

typedef struct
{
	/* These defines physical pins which stepper motor is attached. */
	const uint8_t coil_a_pos_pwma_pin;		/* Often RED wire*/
	const uint8_t coil_a_neg_pwma_pin;		/* Often BLUE wire*/
	const uint8_t coil_b_pos_pwma_pin;		/* Often YELLOW wire*/
	const uint8_t coil_b_neg_pwma_pin;		/* Often WHITE wire*/

	/* Define PWMA channel for each coil pin */
	const uint8_t coil_a_pos_pwma_channel;
	const uint8_t coil_a_neg_pwma_channel;
	const uint8_t coil_b_pos_pwma_channel;	
	const uint8_t coil_b_neg_pwma_channel;
	
	/* This should be common for all PWMA channels. */
	const uint8_t pwma_function;

	/* This tells module of program that currently is controlling motors */
	motorc_ctrl_unit_t ctrl_unit;

	/* This tells direction to move in step array, if move take previous values
	 * from step array, then we move backwards. If we take next value we go forward. */
	motorc_dir_t  motor_direction;
	
	/* We must keep count of steps, this because we must know current index
	 * in step array to know which PWM values should be set next. */
	uint32_t step_count;
	
	/* Step frequency, how many us have to pass before motor
	 * takes one step. This controls speed of motors. 
	 * If value is not NULL it means we should update new value
	 * to TC registers. */
	uint32_t step_delay_ms;
	uint32_t last_step_time_ms;
} motorc_t;

/*!! This is ISR which is used to togle PWM values !!*/
extern void motion_irq(void);

#endif /* MOTION_CONFIG_H_ */