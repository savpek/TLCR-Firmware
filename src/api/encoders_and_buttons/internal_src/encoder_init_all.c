/*
 * encoder_init_all.c
 *
 * Created: 30.9.2011 11:43:23
 *  Author: savpek
 */ 

#include "compiler.h"
#include "encoder.h"
#include "gpio.h"
#include "encoder_config.h"
#include "encoders_and_buttons_isr.h"


void encoder_init_all ()
	{
	uint8_t isr_counter = 0;
	
	Disable_global_interrupt();
	/* Initialization loop, this loop inits all required IO and interrupts
	 * for EIC module and pins used to read encoders.  */	
	while(isr_counter < ENCODER_COUNT)
		{
		/* Init io for interrupt line and state poll line. */
		gpio_enable_gpio_pin(encoder_handle[isr_counter].a_pin);
		gpio_enable_gpio_pin(encoder_handle[isr_counter].b_pin);
		
		/* Set pullup for both gpio channels. */
		gpio_enable_pin_pull_up(encoder_handle[isr_counter].a_pin);
		gpio_enable_pin_pull_up(encoder_handle[isr_counter].b_pin);
			
		/* Init interrupt from encoder A line */
		gpio_enable_pin_interrupt(encoder_handle[isr_counter].a_pin, GPIO_FALLING_EDGE);
		gpio_disable_pin_interrupt(encoder_handle[isr_counter].b_pin);
		
		INTC_register_interrupt(&encoders_and_buttons_isr, 
			AVR32_GPIO_IRQ_0 + (encoder_handle[isr_counter].a_pin/8),
			AVR32_INTC_INT0);			
		isr_counter++;
		}
	Enable_global_interrupt();
	}