/*
 * motion.h
 *
 * Created: 8.8.2011 20:02:02
 *  Author: savpek
 */ 

#ifndef MOTION_H_
#define MOTION_H_

#include "compiler.h"
#include "./api/motion/config/motion_config.h"
#include "./api/errorh/inc/errorh.h"

/* Inits necessary system components for motion library. EG: Clocks and interrupts. */
void motion_init (void);

/*!! Go see source file motion_motor_config.c to config motors used. 
 * In this array, you define all motor parameters and it also includes
 * handle variables that can be written from elsewhere (like change speed and
 * direction. !!*/
extern volatile motorc_t motion_handle[];

/*@ This function tries take control of motor. */
extern errorc_t motion_try_take_control(uint8_t motion_handle_id, motion_access_t);

/*@ This function take control over motor, no matter what unit
 *  is currently controlling it. */
extern void motion_force_take_control(uint8_t motion_handle_id, motion_access_t);

/*@ Checks that which unit is currently using motors */
extern motion_access_t motion_check_control(uint8_t motion_handle_id);

/*@ Release control of unit */
extern void motion_release_control(uint8_t motion_handle_id);

/*@ Sets speed of selected motor.
 *	Units per h are mm/h in X movement, and 1/100 degrees per h
 *	in rotation movement. */
extern void motion_set_rotate_speed(uint8_t motion_handle_id, uint32_t hundred_of_degree_per_h);
extern void motion_set_move_speed(uint8_t motion_handle_id, uint32_t mm_per_h);

/*@ Set direction of selected motor. */
extern void motion_set_direction( uint8_t motion_handle_id, motion_dir_t direction);
#endif /* MOTION_H_ */