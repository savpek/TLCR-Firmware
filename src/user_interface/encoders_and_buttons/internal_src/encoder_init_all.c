/*
 * encoder_init_all.c
 *
 * Created: 30.9.2011 11:43:23
 *  Author: savpek
 */ 

#include "compiler.h"
#include "encoder.h"
#include "eic.h"

void encoder_init_all ()
	{
	uint8_t isr_counter = 0;
	
	eic_options_t eic_encoder_settings[0] =
		{
		/* These are common settings for all
		 * EIC interrupt lines when reading encoder */
		.eic_line = EIC_MODE_EDGE_TRIGGERED,
		.eic_edge = EIC_EDGE_RISING_EDGE,
		.eic_filter = EIC_FILTER_ENABLED,
		.eic_async = EIC_SYNCH_MODE,
		
		/* Defined in initialization loop, not here. */
		.eic_line = 0
		};

	/* Initialize interrupt vectors, this may be called earlier
		* in same program, but no matter we will dot it again for sure */
	INTC_init_interrupts();

	/* Initialization loop, this loop inits all required IO and interrupts
	 * for EIC module and pins used to read encoders.  */	
	while(isr_counter <= sizeof(encoder_handle)/sizeof(encoder_handle[0]))
		{
		/* Init io for interrupt line and state poll line. */
		gpio_enable_module_pin(encoder_handle[isr_counter].a_pin, 
			encoder_handle[isr_counter].a_function, );
		gpio_configure_pin(encoder_handle[isr_counter].b_pin, GPIO_DIR_INPUT&GPIO_PULL_UP);
		
		/* Get irq line settings from encoder_handle and write them to
		 * next EIC settings to set it up */
		eic_encoder_settings[0].eic_line = encoder_handle[isr_counter].irq;
		
		/* Init EIC handler. */
		eic_init(&AVR32_EIC, eic_encoder_settings, 1);

		// Register the EIC interrupt handlers to the interrupt controller.
		INTC_register_interrupt(&eic_int_handler1, 
			EXT_INT_EXAMPLE_IRQ_LINE1, AVR32_INTC_INT0);
		
		isr_counter++;
		}	
	}