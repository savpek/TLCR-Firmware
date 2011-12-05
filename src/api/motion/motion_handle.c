/*
 * motion_handle.c
 *
 * Created: 29.8.2011 12:46:12
 *  Author: savpek
 */ 

#include "./api/motion/inc/motion.h"
#include "./api/motion/config/motion_config.h"
#include "./api/errorh/inc/errorh.h"

/*@ This function takes control of selected motor. */
errorc_t motion_try_take_control(uint8_t motion_handle_id, motorc_ctrl_unit_t new_ctrl_unit)
	{
	/* Check that handle exists. */
	if(motion_handle_id > MOTION_LAST_MOTOR_ID)	return EC_OUT_OF_RANGE;
	
	/* If handle is free, take control */
	if(motion_check_control(motion_handle_id) == new_ctrl_unit) 
		{
		motion_handle[motion_handle_id].ctrl_unit = new_ctrl_unit;
		return EC_SUCCESS;
		}
	/* If not */
	else
		{
		return EC_FAILURE;
		}
	}

/*@ Take control over motors, no matter what */
void motion_force_take_control(uint8_t motion_handle_id, motorc_ctrl_unit_t new_ctrl_unit)
	{
	motion_handle[motion_handle_id].ctrl_unit = new_ctrl_unit;
	}

/*@ Checks that which unit is currently using motors */
motorc_ctrl_unit_t motion_check_control(uint8_t motion_handle_id)
	{
	return motion_handle[motion_handle_id].ctrl_unit;
	}

/*@ Release control handle. */
void motion_release_control(uint8_t motion_handle_id)
	{
	motion_handle[motion_handle_id].ctrl_unit = FREE;
	}

/*@ Sets speed of selected motor.
 *	Units per h are mm/h in X movement, and 1/100 degrees per h
 *	in rotation movement. 
 *	When speed is set, always stops at same time. Movement begins by calling
 *	motion_set_direction and set direction. */
void motion_set_rotate_speed(uint8_t motion_handle_id, uint32_t hundred_of_deg_per_h)
	{
	motion_handle[motion_handle_id].motor_direction = STOP;
	motion_handle[motion_handle_id].step_delay_ms = hundred_of_deg_per_h;	
	}
	
void motion_set_move_speed(uint8_t motion_handle_id, uint32_t mm_per_h)
	{	
	motion_handle[motion_handle_id].motor_direction = STOP;
	
	/* Formula to calculate how long delay should be between steps:
	 * (MS per H)*(millimeter per step)/(millimeter per hour) */
	motion_handle[motion_handle_id].step_delay_ms = 
		(uint32_t)((((MOTION_MS_PER_HOUR*MOTION_UM_PER_STEP_XMOTOR)/1000)/(mm_per_h)));
	
	}

/*@ Set direction of selected motor and start moving with speed set before. */
void motion_set_direction( uint8_t motion_handle_id, motorc_dir_t direction)
	{
	motion_handle[motion_handle_id].motor_direction = direction;	
	}