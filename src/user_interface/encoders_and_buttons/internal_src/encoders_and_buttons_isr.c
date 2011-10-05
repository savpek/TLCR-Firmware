/*
 * encoders_and_buttons_isr.c
 *
 * Created: 2.10.2011 11:03:17
 *  Author: savpek
 */
#include "compiler.h"
#include "gpio.h"
#include "encoders_and_buttons_isr.h"
#include "delay.h"

__attribute__((__interrupt__))
void encoders_and_buttons_isr()
	{
	/* Wait briefly, this must be done because
	 * encoders have very much spikes on voltage level
	 * before they reach stable state. */
	delay_ms(10);
	
	/*!! ENCODERS */
	uint8_t counter = 0;

	gpio_clear_pin_interrupt_flag(10);
	/* Loop through encoder handlers to check which 
	 * encoder caused interrupt */
	while(counter < ENCODER_COUNT)
		{
		/* Checks that is this the one that caused interrupt. Also checks
		 * that interrupt line is truly on high state, this must be checked
		 * because there are voltage spikes when interrupt line drops low too.
		 * And they cause "rising" edge interrupts occassionally. Delay alone
		 * cannot remove this problem. */
		if(gpio_get_pin_interrupt_flag(encoder_handle[counter].a_pin) == 1)
			{
			/* Flag which shows interrupt that happened must be cleared. */
			gpio_clear_pin_interrupt_flag(encoder_handle[counter].a_pin);	

			/* Voltage level error filter check. */
			if(gpio_get_pin_value(encoder_handle[counter].a_pin) == 1) return;
			
			/* If it is, check which direction encoder have been rotated.
			 * this happen by checking encoder B line current state  */
			if(gpio_get_pin_value(encoder_handle[counter].b_pin) == 1)
				{
				encoder_handle[counter].step_count++;
				}
			else if(encoder_handle[counter].step_count > 0)
				{
				encoder_handle[counter].step_count--;
				}				

			/* We can exit interrupt now, since we have done
			 * is required. */
			return;
			} /* if(gpio_get..*/		
		counter++;	
		} /* While */
	
	///*!! BUTTONS */
	///* If none of encoders caused interrupt, lets check
	 //* buttons. First find that did any of button lines cause
	 //* interrupt: */
	//uint8_t int_button_row_pin = 0;
	//counter = 0;
	//while(counter < BUTTON_COUNT &&
		//gpio_get_pin_interrupt_flag(button_handle[counter].row_int_pin) != 1)
		//{
		//counter++;
		//} /* while(int_ ... */
	//
	///* This checks that there truly was interrupt line. */
	//if (gpio_get_pin_interrupt_flag(button_handle[counter].row_int_pin) != 1)
		//{
		///* If not, exit ISR */
		//return;
		//}
	//
	///* Clear interrupt flag, we don't need it anymore at this point. */
	//gpio_clear_pin_interrupt_flag(button_handle[counter].row_int_pin);
	//
	///* Save pin that caused interrupt for later use */
	//int_button_row_pin = button_handle[counter].row_int_pin;
	//
	///* Now as we know button row that caused interrupt, lets find
	 //* correct button from end of that line. For this, we must pull all
	 //* poll lines to high impedance state. Pull one line down, check is
	 //* interrupt row down, if not, continue as long as needed. */
	//counter = 0;
	//while( counter < BUTTON_COUNT)
		//{
		///* Column poll pin state to high. */
		//gpio_set_gpio_pin(button_handle[counter].col_pol_pin);
		//counter++;
		//}
	//
	///* Now, interrupt line is for high state for sure, since all
	 //* pol lines are driven high too. Next we start dropping pol (column)
	 //* lines down one by one, and check everytime interrupt line state. */
	//counter = 0;
	//while( counter < BUTTON_COUNT )
		//{
		//gpio_clr_gpio_pin(button_handle[counter].col_pol_pin);
		//if(gpio_get_pin_value(int_button_row_pin) == 0)
			//{
			///* At this point, we have find correct button */
			//button_handle[counter].button_pressed_flag = 1;
			//break;
			//}
		//counter++;
		//}
		//
	///* Now we must set rest of col (pol) lines to ground for
	 //* waiting next button press. */
	//while ( counter < BUTTON_COUNT )
		//{
		//gpio_clr_gpio_pin(button_handle[counter].col_pol_pin);
		//counter++;
		//}
	//
	///* We are done now */
	}