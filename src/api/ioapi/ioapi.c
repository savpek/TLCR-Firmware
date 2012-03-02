#include "./ioapi/public/ioapi.h"
#include "./gpio/public/gpio.h"
#include "./utest/public/utest.h"

static uint8_t get_port_idx(uint8_t pin) {
	/* This because AVR32 port has 32 pins. */
	return pin>>5;
}

static uint32_t get_pin_bit_in_port(uint8_t pin) {
	return (1<<(pin & 0x1F));
}

static uint8_t is_register_set(uint32_t register_value, uint8_t pin) {
	return ((register_value & get_pin_bit_in_port(pin)) != 0 );
}

uint32_t ioapi_get_pin_info( const uint8_t pin_number ) {
	volatile avr32_gpio_port_t *gpio_port = &AVR32_GPIO.port[get_port_idx(pin_number)];
	uint32_t return_flags = 0;

	if(is_register_set(gpio_port->gper, pin_number)) {
		return_flags |= IOAPI_GPIO_USED;
	}
	else {
		return_flags |= IOAPI_PERIPHEAL_USED;
	}

	if(is_register_set(gpio_port->oder, pin_number)) {
		return_flags |= IOAPI_OUTPUT;
	}
	else {
		return_flags |= IOAPI_INPUT;
	}

	if(is_register_set(gpio_port->ovr, pin_number)) {
		return_flags |= IOAPI_OUT_HIGH;
	}
	else {
		return_flags |= IOAPI_OUT_LOW;
	}

	if(is_register_set(gpio_port->puer, pin_number)) {
		return_flags |= IOAPI_PULL_UP;
	}

	return return_flags;
}

void ioapi_output_high( uint8_t pin_number )
{
	/*	There are functions in GPIO driver which automatically
	 *	init ports when they are used. However, they open risk that
	 *	another part of program accidently overdrives something it shouldn't.
	 *	Now we init all ports in same place, and if port isn't correctly
	 *	initialized we raise ASSERT here! */

	gpio_set_pin_high(pin_number);

	/* Check that driver is set correctly. */
	ERRORH_ASSERT( (ioapi_get_pin_info(pin_number) & IOAPI_GPIO_USED) != 0 );
	ERRORH_ASSERT( (ioapi_get_pin_info(pin_number) & IOAPI_OUTPUT ) != 0);
}

void ioapi_output_low( uint8_t pin_number )
{
	gpio_set_pin_low(pin_number);

	/* Check that driver is set correctly. */
	ERRORH_ASSERT( (ioapi_get_pin_info(pin_number) & IOAPI_GPIO_USED) != 0 );
	ERRORH_ASSERT(ioapi_get_pin_info(pin_number) & IOAPI_OUTPUT);
}

errorc_t ioapi_input_value( uint8_t pin_number )
{
	if(gpio_get_pin_value(pin_number) == 0)
		return EC_FALSE;
	return EC_TRUE;
}
