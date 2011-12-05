/*
 * motion_init.c
 *
 * Created: 21.8.2011 14:35:09
 *  Author: savpek
 */ 

#include "./api/motion/inc/motion.h"
#include "./api/motion/config/motion_config.h"
#include "./api/mcu/inc/mcu.h"
#include "scif_uc3l.h"
#include "pwma.h"
#include "gpio.h"
#include "tc.h"
#include "intc.h"

/* Init motion. Requires init following services:
 *		Interrupt routines X direction.
 *		Interrupt routines ROTATE.
 *		Clock for PWMA (Generic clock 3)
 *		PWMA init. */
void motion_init ()
	{
	uint8_t loopc = 0;
		
	/* These clocks are for PWMA driver, PWMA uses generic clock 3.
	 * We try set PWM frequency neart 50kHz, since it widely accepted good
	 * PWM frequency for steppermotors. */
	scif_gc_setup(AVR32_SCIF_GCLK_PWMA, AVR32_SCIF_GC_USES_CLK_CPU,  MOTION_PWMA_ENABLE_CLK_DIV,
				  MOTION_PWMA_DIVISION_FACTOR);
				  
	scif_gc_enable(AVR32_SCIF_GCLK_PWMA);
	
	/* Next we init all IO and PWMA channels required for motion controller */
	while(loopc <= MOTION_LAST_MOTOR_ID)
		{
		/* Inits A coil positive PWMA */
		pwma_config_and_enable(&AVR32_PWMA,
				(1 << motion_handle[loopc].coil_a_pos_pwma_channel), MOTION_PWMA_PERIOD, 0);
			
		/* Inits A coil negative PWMA */
		pwma_config_and_enable(&AVR32_PWMA,
				(1 << motion_handle[loopc].coil_a_neg_pwma_channel), MOTION_PWMA_PERIOD, 0);				


		/* Inits B coil positive PWMA */
		pwma_config_and_enable(&AVR32_PWMA,
				(1 << motion_handle[loopc].coil_b_pos_pwma_channel), MOTION_PWMA_PERIOD, 0);
			
		/* Inits B coil negative PWMA */
		pwma_config_and_enable(&AVR32_PWMA,
				(1 << motion_handle[loopc].coil_b_neg_pwma_channel), MOTION_PWMA_PERIOD, 0);
		/* !All PWMS are set to "0" value, which means they are off. */
			
		/* Next we init IO for PWMA. */
		gpio_enable_module_pin(motion_handle[loopc].coil_a_pos_pwma_pin, 
								motion_handle[loopc].pwma_function);
		gpio_enable_module_pin(motion_handle[loopc].coil_a_neg_pwma_pin, 
								motion_handle[loopc].pwma_function);
		gpio_enable_module_pin(motion_handle[loopc].coil_b_pos_pwma_pin, 
								motion_handle[loopc].pwma_function);
		gpio_enable_module_pin(motion_handle[loopc].coil_b_neg_pwma_pin, 
								motion_handle[loopc].pwma_function);
								
		loopc++;
		}
		
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
	INTC_register_interrupt(&motion_irq, AVR32_TC0_IRQ0, AVR32_INTC_INT0);

	/* We init TC with values set before */
	tc_init_waveform((&AVR32_TC0), &WAVEFORM_OPT);
	
	
	tc_configure_interrupts((&AVR32_TC0), 0, &TC_INTERRUPT);
	
	tc_write_rc((&AVR32_TC0), 0, 3000); /* This sets compare register value,
										 * when counter reaches this value, interrupt is triggered. */
	
	/* Then we start TC0 channel 0. */
	tc_start((&AVR32_TC0), 0);
	
	}