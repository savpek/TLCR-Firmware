/*
 * motion_rdir_thread.c
 *
 * Created: 21.8.2011 14:34:55
 *  Author: savpek
 */ 

#include "compiler.h"
#include "motion.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwma.h"
#include "io_config.h"
#include "gpio.h"
#include "usart.h"
#include "terminal.h"
#include "motion_config.h"
#include "tc.h"
#include "intc.h"

static void _delay()
	{
	int a = 0;
	while(a<1000) a++;
	}

static void _init_tc(uint8_t tc_channel)
	{
	
	}

void motion_thread(void* motor_parameters)
	{
	motorc_t *motor_settings;
	motor_settings = (motorc_t*) motor_parameters;
	
	portTickType rtos_last_wakeup;
		
	/* Inits A coil positive PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_settings->coil_a_pos_pwma), MOTION_PWMA_PERIOD, 0);
			
	/* Inits A coil negative PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_settings->coil_a_neg_pwma), MOTION_PWMA_PERIOD, 0);				


	/* Inits B coil positive PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_settings->coil_a_pos_pwma), MOTION_PWMA_PERIOD, 0);
			
	/* Inits B coil negative PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_settings->coil_a_neg_pwma), MOTION_PWMA_PERIOD, 0);
	/* !All PWMS are set to "0" value, which means they are off. */
			
	/* Next we init IO for PWMA. */
	gpio_enable_module_pin(motor_settings->coil_a_pos_pwma, PWMA_RMOTOR_FUNCTION);
	gpio_enable_module_pin(motor_settings->coil_a_neg_pwma, PWMA_RMOTOR_FUNCTION);
	gpio_enable_module_pin(motor_settings->coil_b_pos_pwma, PWMA_RMOTOR_FUNCTION);
	gpio_enable_module_pin(motor_settings->coil_b_neg_pwma, PWMA_RMOTOR_FUNCTION);
	
	while(1)
		{
	
		}		
	}
