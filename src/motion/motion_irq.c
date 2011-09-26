/*
 * motion_irqs.c
 *
 * Created: 28.8.2011 17:04:08
 *  Author: savpek
 */ 
#include "motion_config.h"
#include "motion.h"
#include "usart.h"
#include "terminal.h"
#include "tc.h"
#include "pwma.h"

static volatile uint32_t _ms_counter = 0;

/* Setup A coil value, value can be from -127 to +127. That is value negative positive
 * or negative tells coil current direction */
static void _a_coil_value (volatile motorc_t* motor_params, int8_t coil_value)
	{
	/* If value is >0, it means that coil direction is positive */
	if(coil_value >= 0)
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_a_neg_pwma_channel), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_a_pos_pwma_channel), coil_value);
		}
	/* Otherwise it should be negative */
	else
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_a_pos_pwma_channel), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_a_neg_pwma_channel), coil_value*(-1));			
		}
	}

/* Setup B coil value, value can be from -127 to +127. That is value negative positive
 * or negative tells coil current direction */
static void _b_coil_value (volatile motorc_t* motor_params, int8_t coil_value)
	{
	/* If value is >0, it means that coil direction is positive */
	if(coil_value >= 0)
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_b_neg_pwma_channel), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_b_pos_pwma_channel), coil_value);
		}
	/* Otherwise it should be negative */
	else
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_b_pos_pwma_channel), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->coil_b_neg_pwma_channel), coil_value*(-1));			
		}
	}

/* Interrupt routine that controls motor step routines. 
 * In shortly moves in next PWM step sequence when needed. */
__attribute__((__interrupt__))
void motion_irq()
	{
	uint8_t loop_counter = 0;
		
	_ms_counter++;
	
	/* This is required to reset RC interrupt flag */
	tc_read_sr((&AVR32_TC0), 0);
	
	/* We check all motors from list, if their movement delay matches
	 * ms counter, take step */
	while(loop_counter <= MOTION_LAST_MOTOR_ID)
		{
		/* Check current motor, is step suppose to happen now. */
		if(_ms_counter%motion_handle[loop_counter].step_delay_ms == 0
			&& motion_handle[loop_counter].motor_direction != STOP)
			{
			/* We need last momen of step to sleep down motor currents later */	
			motion_handle[loop_counter].last_step_time_ms = _ms_counter;
			
			/* Step counter is required for each motor to know
			 * which step should happen next. .motor_direction variable is either
			 * +1 or -1 */
			motion_handle[loop_counter].step_count += motion_handle[loop_counter].motor_direction;
			
			/* Setup coil values */
			_a_coil_value(&motion_handle[loop_counter], 
				motion_step_sequence[ motion_handle[loop_counter].step_count % MOTION_MICROSTEP_COUNT ].coil_a_pwma_value);

			_b_coil_value(&motion_handle[loop_counter],
				motion_step_sequence[ motion_handle[loop_counter].step_count % MOTION_MICROSTEP_COUNT ].coil_b_pwma_value);
			}
		
		/* Stepper motor "holds" it's position with high torgue, that requires a lot of
		 * power and it heats up the motor. To increase time that device can be used with single
		 * battery, we must drop holding torque lower. */
		if(motion_handle[loop_counter].last_step_time_ms+MOTION_SLEEP_DELAY_MS == _ms_counter)
			{
			_a_coil_value(&motion_handle[loop_counter], 
				motion_step_sleep_sequence[ motion_handle[loop_counter].step_count % MOTION_MICROSTEP_COUNT ].coil_a_pwma_value);

			_b_coil_value(&motion_handle[loop_counter],
				motion_step_sleep_sequence[ motion_handle[loop_counter].step_count % MOTION_MICROSTEP_COUNT ].coil_b_pwma_value);
			}
		
		/* Now move to next motor in motion_handle array */
		loop_counter++;
		}
	}