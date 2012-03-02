/*
 * motion.h
 *
 * Created: 8.8.2011 20:02:02
 *  Author: savpek
 
	High level API for TLCR motion control. Uses motor driver to control motors.
 */ 

#ifndef MOTION_H_
#define MOTION_H_

#include "compiler.h"
#include "./motion/config/motion_config.h"
#include "./errorh/inc/errorh.h"

typedef enum
	{
	MOTION_ACCESS_FREE = 0,
	MOTION_ACCESS_SCRIPT = 1,
	MOTION_ACCESS_TERMINAL = 3,
	} motion_access_t;

/* Inits motion library. Basically connects your motor settings
 * to motor driver and inits mutex for FreeRTOS. */
extern void motion_init (void);

/* Run motors, 0 speed is stopped. */
extern errorc_t motion_rotate(int32_t speed_md_h, motion_access_t app_access);
extern errorc_t motion_move(int32_t speed_um_h, motion_access_t app_access);

extern errorc_t motion_release(motion_access_t app_access);

#endif /* MOTION_H_ */