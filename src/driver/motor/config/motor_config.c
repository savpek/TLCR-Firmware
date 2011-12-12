/*
 * motion_motor_config.c
 *
 * Created: 28.8.2011 15:50:30
 *  Author: savpek
 */ 

#include "compiler.h"

#include "./driver/motor/config/motor_config.h"

const motor_step_t motor_step[MOTOR_STEP_COUNT] = {
 /*  A-coil     B-coil		<-PWMA value */
	{-127,		-127},		//STEP 0
	{127,		-127},		//STEP 1
	{127,		127},		//STEP 2
	{-127,		127},		//STEP 3
};

const motor_step_t motor_sleep_step[MOTOR_STEP_COUNT] = {
 /*  A-coil     B-coil		<-PWMA value */
	{-60,		-60},		//STEP 0
	{60,		-60},		//STEP 1
	{60,		60},		//STEP 2
	{-60,		60},		//STEP 3
};