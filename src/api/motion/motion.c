/*
 * motion.c
 *
 * Created: 13.12.2011 10:53:46
 *  Author: savpek
 */ 

#include "compiler.h"

#include "./api/motion/inc/motion.h"
#include "./api/motion/config/motion_config.h"
#include "./driver/motor/inc/motor.h"

volatile motor_t l_motors[2] =
	{
	/* MOTION_XMOTOR (0): ... 
	 * This motor moves camera left/right */
		{
		/* Define physical pins for PWMA */
		.a_pos_pwma_pin = MOTION_XMOTOR_A_POS_PIN,	/* Often RED wire*/
		.a_neg_pwma_pin = MOTION_XMOTOR_A_NEG_PIN,	/* Often BLUE wire*/
		.b_pos_pwma_pin = MOTION_XMOTOR_B_POS_PIN,	/* Often YELLOW wire*/
		.b_neg_pwma_pin = MOTION_XMOTOR_B_NEG_PIN,	/* Often WHITE wire*/

		/* Define PWMA channel for each coil pin */
		.a_pos_pwma_chan = MOTION_XMOTOR_A_POS_CHAN,
		.a_neg_pwma_chan = MOTION_XMOTOR_A_NEG_CHAN,
		.b_pos_pwma_chan = MOTION_XMOTOR_B_POS_CHAN,
		.b_neg_pwma_chan = MOTION_XMOTOR_B_NEG_CHAN,
		.pwma_function = MOTION_PWMA_FUNCTION,
		},
	/* MOTION_RMOTOR (1): ... 
	 * This motor rotates camera. */
		{
		/* Define physical pins for PWMA */
		.a_pos_pwma_pin = MOTION_RMOTOR_A_POS_PIN,	/* Often RED wire*/
		.a_neg_pwma_pin = MOTION_RMOTOR_A_NEG_PIN,	/* Often BLUE wire*/
		.b_pos_pwma_pin = MOTION_RMOTOR_B_POS_PIN,	/* Often YELLOW wire*/
		.b_neg_pwma_pin = MOTION_RMOTOR_B_NEG_PIN,	/* Often WHITE wire*/

		/* Define PWMA channel for each coil pin */
		.a_pos_pwma_chan = MOTION_RMOTOR_A_POS_CHAN,
		.a_neg_pwma_chan = MOTION_RMOTOR_A_NEG_CHAN,
		.b_pos_pwma_chan = MOTION_RMOTOR_B_POS_CHAN,
		.b_neg_pwma_chan = MOTION_RMOTOR_B_NEG_CHAN,
		.pwma_function = MOTION_PWMA_FUNCTION,
		}
	};

volatile motion_access_t l_cur_access;

/* Inits motion library. Basically connects your motor settings
 * to motor driver and inits mutex for FreeRTOS. */
void motion_init (void)
	{
	/* TODO: MUTEX INIT*/
	
	/* Init motors */
	motor_init(&l_motors[0]);
	motor_init(&l_motors[1]);
	
	/* Set current motor acces stationary (free). */
	l_cur_access = MOTION_ACCESS_FREE;
	}

/* Run motors, 0 speed is stopped. */
errorc_t motion_rotate(int32_t speed_md_h, motion_access_t app_access)
	{
	/* TODO: GET MUTEX */
	
	/* Check access */
	if(app_access >= l_cur_access)
		{
		l_cur_access = app_access; 
		}
	else
		{
		/* TODO: RELEASE MUTEX */
		return EC_ACCESS_DENIED;	
		}
			
	/* How many ms step need to delay to archieve
	 * defined mD/h speed */
	uint32_t step_delay = 0;
	
	if(speed_md_h < 0)
		{
		/* Convert negative to positive speed value */
		speed_md_h *= -1; 
		
		/* Calculate delay between step needed */
		step_delay = ( 3600000UL / ( speed_md_h / MOTION_1_1000_OF_DEG_PER_STEP_RMOTOR ) );
			
		motor_step_delay(&l_motors[MOTION_RMOTOR], step_delay, MOTOR_COUNTERCW);
		}
	else
		{
		/* Calculate delay between step needed */
		step_delay = ( 3600000UL / ( speed_md_h / MOTION_1_1000_OF_DEG_PER_STEP_RMOTOR ) );
			
		motor_step_delay(&l_motors[MOTION_RMOTOR], step_delay, MOTOR_CW);			
		}
	
	/* TODO: PUT MUTEX */
	
	return EC_SUCCESS;
	}
	
errorc_t motion_move(int32_t speed_um_h, motion_access_t app_access)
	{
	/* TODO: GET MUTEX */
	
	/* Check access */
	if(app_access >= l_cur_access)
		{
		l_cur_access = app_access; 
		}
	else
		{
		/* TODO: RELEASE MUTEX */
		return EC_ACCESS_DENIED;	
		}
	
	/* How many ms step need to delay to archieve
	 * defined um/h speed */
	uint32_t step_delay = 0;
	
	if(speed_um_h < 0)
		{
		/* Convert negative to positive speed value */
		speed_um_h *= -1; 
		
		/* Calculate delay between step needed */
		step_delay = ( 3600000UL / ( speed_um_h / MOTION_UM_PER_STEP_XMOTOR ) );
			
		motor_step_delay(&l_motors[MOTION_XMOTOR], step_delay, MOTOR_COUNTERCW);
		}
	else
		{
		/* Calculate delay between step needed */
		step_delay = ( 3600000UL / ( speed_um_h / MOTION_UM_PER_STEP_XMOTOR ) );
					motor_step_delay(&l_motors[MOTION_XMOTOR], step_delay, MOTOR_CW);			
		}
	
	/* TODO: RELEASE MUTEX */
	
	return EC_SUCCESS;	
	}

/* This function releases control over motion */
errorc_t motion_release(motion_access_t app_access)
	{
	/* If current app have same or higher access than
	 * old one, free it. */
	if(app_access >= l_cur_access)
		{
		l_cur_access = MOTION_ACCESS_FREE;
		return EC_SUCCESS;
		}
	else
		{
		return EC_ACCESS_DENIED;	
		}
	}