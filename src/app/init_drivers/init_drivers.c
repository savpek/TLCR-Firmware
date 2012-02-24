/*
 * init_drivers.c
 *
 * Created: 24.2.2012 11:40:51
 *  Author: savpek
 */ 

#include "./init_drivers/public/init_drivers.h"
#include "./init_drivers/private/init_drivers_private.h"

void init_drivers( ) {
	init_driver_clocks();
	init_driver_gpio();
	init_driver_usart();
}