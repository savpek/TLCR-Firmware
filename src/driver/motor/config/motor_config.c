/*
 * motion_motor_config.c
 *
 * Created: 28.8.2011 15:50:30
 *  Author: savpek
 */ 

#include "compiler.h"

#include "./driver/motor/config/motor_config.h"

#define M_VP	80
#define M_VN	80


const motor_step_t motor_step[MOTOR_STEP_COUNT] = {
 /*  A-coil     B-coil		<-PWMA value */
	{-127,		-127},		//STEP 0
	{-95,		-127},		//STEP 0
	{-63,		-127},		//STEP 0
	{0,			-127},		//STEP 0		
	{63,		-127},		//STEP 0
	{95,		-127},		//STEP 0
	{127,		-127},		//STEP 3
	{127,		-95},		//STEP 1
	{127,		-63},		//STEP 1
	{127,		0	},		//STEP 0
	{127,		63},		//STEP 3
	{127,		95},		//STEP 2
	{127,		127},		//STEP 3
	{95,		127},		//STEP 2
	{63,		127},		//STEP 3
	{0,		127},			//STEP 0
	{-63,		127},		//STEP 3		<
	{-95,		127},		//STEP 3
	{-127,		95},		//STEP 3
	{-127,		63},		//STEP 3
	{-127,		0},			//STEP 0
	{-127,		-63},		//STEP 3
	{-127,		-95},		//STEP 3
};

const motor_step_t motor_sleep_step[MOTOR_STEP_COUNT] = {
 /*  A-coil     B-coil		<-PWMA value */
	{-70,		-70},		//STEP 0
	{-65,		-70},		//STEP 0
	{-58,		-70},		//STEP 0
	{0,			-70},		//STEP 0		
	{58,		-70},		//STEP 0
	{65,		-70},		//STEP 0
	{70,		-70},		//STEP 3
	{70,		-65},		//STEP 1
	{70,		-58},		//STEP 1
	{70,		0	},		//STEP 0
	{70,		58},		//STEP 3
	{70,		65},		//STEP 2
	{70,		70},		//STEP 3
	{65,		70},		//STEP 2
	{58,		70},		//STEP 3
	{0,		70},			//STEP 0
	{-58,		70},		//STEP 3		
	{-65,		70},		//STEP 3
	{-70,		65},		//STEP 3
	{-70,		58},		//STEP 3
	{-70,		0},			//STEP 0
	{-70,		-58},		//STEP 3
	{-70,		-65},		//STEP 3
};