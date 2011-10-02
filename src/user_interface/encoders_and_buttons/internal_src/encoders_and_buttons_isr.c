/*
 * encoders_and_buttons_isr.c
 *
 * Created: 2.10.2011 11:03:17
 *  Author: savpek
 */
#include "encoders_and_buttons_isr.h"

__attribute__((__interrupt__))
void encoders_and_buttons_isr()
	{
	uint8_t encoder_counter = 0;
	
	/* Loop through encoder handlers to check which 
	 * encoder caused interrupt */
	while(encoder_counter <= sizeof(encoder_handle)/sizeof(encoder_handle[0]))
		{
		/* Checks that is this the one that caused interrupt */
		if(gpio_get_pin_interrupt_flag(encoder_handle[encoder_counter].a_pin) == 1)
			{
			/* If it is, check which direction encoder have been rotated.
			 * this happen by checking encoder B line current state  */
			if(gpio_get_pin_value(encoder_handle[encoder_counter].b_pin) == 1)
				{
				encoder_handle[encoder_counter].step_count++;
				}
			else if(encoder_handle[encoder_counter].step_count > 0)
				{
				encoder_handle[encoder_counter].step_count--;
				}
			
			/* Flag which shows interrupt that happened must be cleared. */
			gpio_clear_pin_interrupt_flag(encoder_handle[encoder_counter].a_pin);
			
			/* Exit interrupt, we are done. */
			return;
			}
			
		encoder_counter++;	
		}		
	}