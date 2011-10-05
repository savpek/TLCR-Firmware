/*
 * button_init_all.c
 *
 * Created: 2.10.2011 16:19:28
 *  Author: savpek
 */ 
#include "compiler.h"
#include "intc.h"
#include "button.h"
#include "button_config.h"
#include "gpio.h"
#include "encoders_and_buttons_isr.h"

void button_init_all(void)
	{
	Disable_global_interrupt();
	
	/* At first, we should init interrupt lines. */	
	uint8_t counter = 0;
	while(counter < BUTTON_COUNT)
		{
		/* Init io for interrupt line and state poll line. */
		gpio_configure_pin(button_handle[counter].row_int_pin, 
			GPIO_DIR_INPUT);		
		gpio_configure_pin(button_handle[counter].col_pol_pin, 
			GPIO_DIR_OUTPUT);
		
		/* Set pullup for interrupt lines. */
		gpio_enable_pin_pull_up(button_handle[counter].row_int_pin);
		
		/* Set poll pin to gnd */
		gpio_clr_gpio_pin(button_handle[counter].col_pol_pin);
		
		/* Init interrupt from encoder A line */
		gpio_enable_pin_interrupt(button_handle[counter].row_int_pin, GPIO_FALLING_EDGE);
		
		gpio_enable_pin_glitch_filter(button_handle[counter].row_int_pin);
		
		/*INTC_register_interrupt(&encoders_and_buttons_isr, 
			AVR32_GPIO_IRQ_0 + (button_handle[counter].row_int_pin/8), 
			AVR32_INTC_INT0);*/
			
		counter++;
		}

	Enable_global_interrupt();
	}	