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
#define MOTION_XMOTOR	0
#define MOTION_RMOTOR	1

/*!! These constants tells how long distance device moves during one step from
 *   stepper motors. !!*/
#define MOTION_UM_PER_STEP_XMOTOR 15 /* How many um device moves during one step. */
#define MOTION_1_1000_OF_DEG_PER_STEP_RMOTOR 78 /* How many 1/1000 degree camera rotes during one step. */


/* Settings for motors 
 * XMOTOR -> */
#define MOTION_XMOTOR_A_POS_PIN			AVR32_PWMA_28_PIN
#define MOTION_XMOTOR_A_POS_CHAN		28
#define MOTION_XMOTOR_A_NEG_PIN			AVR32_PWMA_29_PIN
#define MOTION_XMOTOR_A_NEG_CHAN		29

#define MOTION_XMOTOR_B_POS_PIN			AVR32_PWMA_30_PIN
#define MOTION_XMOTOR_B_POS_CHAN		30
#define MOTION_XMOTOR_B_NEG_PIN			AVR32_PWMA_31_PIN
#define MOTION_XMOTOR_B_NEG_CHAN		31

/* RMOTOR -> */
#define MOTION_RMOTOR_A_POS_PIN			AVR32_PWMA_24_PIN
#define MOTION_RMOTOR_A_POS_CHAN		24
#define MOTION_RMOTOR_A_NEG_PIN			AVR32_PWMA_25_PIN
#define MOTION_RMOTOR_A_NEG_CHAN		25

#define MOTION_RMOTOR_B_POS_PIN			AVR32_PWMA_26_PIN
#define MOTION_RMOTOR_B_POS_CHAN		26
#define MOTION_RMOTOR_B_NEG_PIN			AVR32_PWMA_27_PIN
#define MOTION_RMOTOR_B_NEG_CHAN		27

#define MOTION_PWMA_FUNCTION			AVR32_PWMA_15_FUNCTION

#endif /* MOTION_CONFIG_H_ */