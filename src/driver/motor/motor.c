/*
 * motor.c
 *
 * Created: 12.12.2011 13:37:32
 *  Author: savpek
 */ 

/* ASF */
#include "scif_uc3l.h"
#include "pwma.h"
#include "gpio.h"
#include "tc.h"
#include "intc.h"

/* INTERNAL */
#include "./driver/motor/inc/motor.h"
#include "./driver/motor/config/motor_config.h"

/* TLCR API */
#include "./api/mcu/inc/mcu.h"

/* Array of pointers to motor_t struct */
volatile uint32_t step_counter = 0;
volatile motor_t *motor[MOTOR_MAX_COUNT+1] = {0};

/* Setup A coil value, value can be from -127 to +127. That is value negative positive
 * or negative tells coil current direction */
static void s_a_coil_value (volatile motor_t *motor_params, int8_t coil_value)
	{
	/* If value is >0, it means that coil direction is positive */
	if(coil_value >= 0)
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->a_neg_pwma_chan), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->a_pos_pwma_chan), coil_value);
		}
	/* Otherwise it should be negative */
	else
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->a_pos_pwma_chan), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->a_neg_pwma_chan), coil_value*(-1));			
		}
	}

/* Setup B coil value, value can be from -127 to +127. That is value negative positive
 * or negative tells coil current direction */
static void s_b_coil_value (volatile motor_t *motor_params, int8_t coil_value)
	{
	/* If value is >0, it means that coil direction is positive */
	if(coil_value >= 0)
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->b_neg_pwma_chan), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->b_pos_pwma_chan), coil_value);
		}
	/* Otherwise it should be negative */
	else
		{
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->b_pos_pwma_chan), 0);
		pwma_set_channels_value( &AVR32_PWMA, (1<<motor_params->b_neg_pwma_chan), coil_value*(-1));			
		}
	}

/* Interrupt routine that controls motor step routines. 
 * In shortly moves in next PWM step sequence when needed. */
__attribute__((__interrupt__))
static void motor_isr( void )
	{
	uint8_t counter = 0;
	
	/* This is required to reset RC interrupt flag */
	tc_read_sr((&AVR32_TC0), 0);
		
	/* We check all motors from list, if their movement delay matches
	 * ms counter, take step */
	while(motor[counter] != 0)
		{
		motor[counter]->delay_counter++;	
			
		/* Check current motor, is step suppose to happen now. */
		if( ( motor[counter]->delay_counter % motor[counter]->delay == 0 ) &&
		      motor[counter]->direction != MOTOR_STOP )
			{
			motor[counter]->step += motor[counter]->direction;

			/* Keep step variable in range */
			if(motor[counter]->step < 0) motor[counter]->step = MOTOR_STEP_COUNT-1;
			if(motor[counter]->step > MOTOR_STEP_COUNT-1) motor[counter]->step = 0;

			motor[counter]->delay_counter = 0;
			
			/* Setup coil values */
			s_a_coil_value(motor[counter],
				motor_step[ motor[counter]->step ].a_pwma_value);

			s_b_coil_value(motor[counter],
				motor_step[ motor[counter]->step ].b_pwma_value);
				
			step_counter++;
			}
		
		/* Stepper motor "holds" it's position with high torgue, that requires a lot of
		 * power and it heats up the motor. To increase time that device can be used with single
		 * battery, we must drop holding torque lower. */
		if( motor[counter]->delay_counter == MOTOR_SLEEP_DELAY_MS )
			{
			s_a_coil_value( motor[counter], 
				motor_sleep_step[ motor[counter]->step ].a_pwma_value);

			s_b_coil_value( motor[counter],
				motor_sleep_step[ motor[counter]->step ].b_pwma_value );
			}
		
		/* Now move to next motor in motion_handle array */
		counter++;
		}
	}

/* Init motor. Requires init following services:
 *		Interrupt routines.
 *		Clock for PWMA (Generic clock 3)
 *		PWMA init. */
errorc_t motor_init (volatile motor_t *motor_handle)
	{
	/* First we put new motor to motor array */
	uint8_t counter = 0;
	while(motor[counter] != 0)
		{
		counter++;
		if(MOTOR_MAX_COUNT <= counter)
			{
			return EC_FULL;	
			}
		}
	
	motor[counter] = motor_handle;
	Disable_global_interrupt();
	/* These clocks are for PWMA driver, PWMA uses generic clock 3.
	 * We try set PWM frequency neart 50kHz, since it widely accepted good
	 * PWM frequency for steppermotors. */
	scif_gc_setup(AVR32_SCIF_GCLK_PWMA, AVR32_SCIF_GC_USES_CLK_CPU,  MOTOR_PWMA_ENABLE_CLK_DIV,
				  MOTOR_PWMA_DIVISION_FACTOR);
				  
	scif_gc_enable(AVR32_SCIF_GCLK_PWMA);
	
	/* Inits A coil positive PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_handle->a_pos_pwma_chan), MOTOR_PWMA_PERIOD, 0);
			
	/* Inits A coil negative PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_handle->a_neg_pwma_chan), MOTOR_PWMA_PERIOD, 0);				


	/* Inits B coil positive PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_handle->b_pos_pwma_chan), MOTOR_PWMA_PERIOD, 0);
			
	/* Inits B coil negative PWMA */
	pwma_config_and_enable(&AVR32_PWMA,
			(1 << motor_handle->b_neg_pwma_chan), MOTOR_PWMA_PERIOD, 0);
	/* !All PWMS are set to "0" value, which means they are off. */
			
	/* Next we init IO for PWMA. */
	gpio_enable_module_pin(	motor_handle->a_pos_pwma_pin, 
							motor_handle->pwma_function);
	gpio_enable_module_pin(	motor_handle->a_neg_pwma_pin, 
							motor_handle->pwma_function);
	gpio_enable_module_pin(	motor_handle->b_pos_pwma_pin, 
							motor_handle->pwma_function);
	gpio_enable_module_pin(	motor_handle->b_neg_pwma_pin, 
							motor_handle->pwma_function);
		
	/* Motion requires TC clock and interrupt with fixed ms frequency.
	 * We are using RC compare interrupt which happens every time when
	 * counter reaches RC value. On UP_AUTO mode counter is automatically
	 * reseted to zero and counting begin again.
	 * Settings for TC clock: */
	static const tc_waveform_opt_t WAVEFORM_OPT =
	{
    .channel  = 0,						// Channel selection.

    .bswtrg   = TC_EVT_EFFECT_NOOP,     // Software trigger effect on TIOB.
    .beevt    = TC_EVT_EFFECT_NOOP,     // External event effect on TIOB.
    .bcpc     = TC_EVT_EFFECT_NOOP,     // RC compare effect on TIOB.
    .bcpb     = TC_EVT_EFFECT_NOOP,     // RB compare effect on TIOB.

    .aswtrg   = TC_EVT_EFFECT_NOOP,     // Software trigger effect on TIOA.
    .aeevt    = TC_EVT_EFFECT_NOOP,     // External event effect on TIOA.
    .acpc     = TC_EVT_EFFECT_NOOP,     // RC compare effect on TIOA: toggle.
    .acpa     = TC_EVT_EFFECT_NOOP,     // RA compare effect on TIOA: toggle.

    .wavsel   = AVR32_TC_WAVSEL_UP_AUTO, /* Trigger(reset) on RC compare, this means
										  * that when counter reaches RC value, it resets it's value to
										  * 0x0000 */
    .enetrg   = false,                   // External event trigger enable.
    .eevt     = 0,                       // External event selection.
    .eevtedg  = TC_SEL_NO_EDGE,          // External event edge selection.
    .cpcdis   = false,                   // Counter disable when RC compare.
    .cpcstop  = false,                   // Counter clock stopped with RC compare.

    .burst    = false,                   // Burst signal selection.
    .clki     = false,                   // Clock inversion.
    .tcclks   = TC_CLOCK_SOURCE_TC3		 /* PBA/8 Clock source. */
	};
	
	static const tc_interrupt_t TC_INTERRUPT =
	 {
    .etrgs = 0,
    .ldrbs = 0,
    .ldras = 0,
    .cpcs  = 1, /* RC compare interrupt */
    .cpbs  = 0,
    .cpas  = 0,
    .lovrs = 0,
    .covfs = 0
	};

	/* IRQ used for controlling motion can be found from motion_irq. file. */
	INTC_register_interrupt(&motor_isr, AVR32_TC0_IRQ0, AVR32_INTC_INT0);

	/* We init TC with values set before */
	tc_init_waveform((&AVR32_TC0), &WAVEFORM_OPT);
	
	tc_configure_interrupts((&AVR32_TC0), 0, &TC_INTERRUPT);
	
	tc_write_rc((&AVR32_TC0), 0, MOTOR_PBA_CYCLES_AS_MS); /* This sets compare register value,
										 * when counter reaches this value, interrupt is triggered. */
	
	/* Then we start TC0 channel 0. */
	tc_start((&AVR32_TC0), 0);
	Enable_global_interrupt();
	}
	
void motor_step_delay(volatile motor_t *motor_handle, uint32_t step_delay, motor_dir_t direction)
	{
	motor_handle->delay = step_delay;
	motor_handle->direction = direction;
	}